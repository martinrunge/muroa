/*
 * CDiffBuilder.cpp
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#include "CDiffBuilder.h"
#include "CPlaylistItem.h"

#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"


CDiffBuilder::CDiffBuilder(CRootItem* mediaColPtr, CRootItem* plPtr, CRootItem* nlPtr ):
    m_mediaColPtr(mediaColPtr),
    m_plPtr(plPtr),
    m_nlPtr(nlPtr)
{

}

CDiffBuilder::~CDiffBuilder() {

}


void CDiffBuilder::prepareDiff(CModelDiff* md)
{
	enum origin dest = md->getDestination();
	enum origin orig = md->getOrigin();

	enum origin commandType;

	int numToRemove = 0;
	int numToInsert = 0;

	md->sort();

	switch(orig)
	{
	case E_MEDIA_COL:
		this->getItemToRemove = &CDiffBuilder::removeFromCollection;
		switch(dest)
		{
		case E_MEDIA_COL:
			// move inside collection
			commandType = E_MEDIA_COL;
			numToRemove = md->getNumSelected();
			numToInsert = numToRemove;
			getItemToInsert = &CDiffBuilder::insertFromCollectionToCollection;

		case E_PLAYLIST:
			// insert songs from collection into playlist
			commandType = E_PLAYLIST;
			numToInsert = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::insertFromCollectionToPlaylist;

			break;

		case E_NEXTLIST:
			commandType = E_NEXTLIST;
			// two possible modes:
			// insert songs from collection into nextlist AND append to playlist
			// insert songs into nextlist only, so they disappear after playing
			numToInsert = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::insertFromCollectionToNextlist;
			break;

		default:
			qDebug() << QString("CDiffBuilder::getText: unknown destination: %1").arg(dest);
		}
		break;

	case E_PLAYLIST:
		getItemToRemove = &CDiffBuilder::removeFromPlaylist;
		switch(dest)
		{
		case E_MEDIA_COL:
			// remove songs from playlist
			commandType = E_PLAYLIST;
			numToRemove = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::removeFromPlaylist;
			break;

		case E_PLAYLIST:
			// move songs inside playlist
			commandType = E_PLAYLIST;
			numToRemove = md->getNumSelected();
			numToInsert = numToRemove;
			getItemToInsert = &CDiffBuilder::insertFromPlaylistToPlaylist;
			break;

		case E_NEXTLIST:
			// insert songs from playlist into nextlist at drop position
			commandType = E_NEXTLIST;
			numToInsert = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::insertFromPlaylistToNextlist;
			break;

		default:
		    qDebug() << QString("CDiffBuilder::getText: unknown destination: %1").arg(dest);
		}
		break;

	case E_NEXTLIST:
		getItemToRemove = &CDiffBuilder::removeFromNextlist;
		switch(dest)
		{
		case E_MEDIA_COL:
		case E_PLAYLIST:
			// remove selected songs from nextlist
			commandType = E_NEXTLIST;
			numToRemove = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::removeFromNextlist;
			break;

		case E_NEXTLIST:
			// move selected songs inside nextlist
			commandType = E_NEXTLIST;
			numToInsert = md->getNumSelected();
			numToRemove = numToInsert;
			getItemToInsert = &CDiffBuilder::insertFromNextlistToNextlist;
			break;

		default:
			qDebug() << QString("CDiffBuilder::getText: unknown destination: %1").arg(dest);
		}
		break;

	default:
		qDebug() << QString("CDiffBuilder::getText: unknown origin: %1").arg(orig);
	}

	md->setCommandType(commandType);
	md->setNumToInsert(numToInsert);
	md->setNumToRemove(numToRemove);
}

std::string CDiffBuilder::diff(CModelDiff md)
{
	std::string text;

	prepareDiff(&md);

	int numToInsert = md.getNumToInsert();
	int numToRemove = md.getNumToRemove();

	int rmFrom = md.getSelectedItems().at(0).line;
	int rmTo = md.getSelectedItems().at( md.getNumSelected() - 1 ).line;

	int insTo = md.getInsertPos();

	// ignore, if rows to remove are to be moved into their own range. (dropped on self)
	if( rmFrom < insTo && rmTo > insTo && md.getOrigin() == md.getDestination() )
	{
		//qDebug() << std::string("dropped on origin -> ignore: [%1, %2] -> %3").arg(rmFrom).arg(rmTo).arg(insTo);
		//qDebug() << std::string("from: %1 to: %2 ").arg(md.getOrigin()).arg(md.getDestination());
		//md.dump();
		return text;
	}
	else {
		//qDebug() << std::string("building diff: [%1, %2] -> %3").arg(rmFrom).arg(rmTo).arg(insTo);
		//md.dump();
	}

	if(rmFrom > insTo)
	{
		// insert first
		if(numToInsert > 0)
		{
			text.append( prepareDiffHeader(insTo, 0, insTo +1, numToInsert) );
			for(int i = 0; i < numToInsert; i++)
			{
				text.append( "+" );
				text.append( (this->*getItemToInsert)(md.getSelectedItems().at(i)) );
				text.append( "\n" );
			}
		}

		// remove then
		if(numToRemove > 0)
		{
			text.append( prepareDiffHeader(rmFrom + 1, numToRemove, rmFrom + numToInsert, 0) );
			for(int i = 0; i < numToRemove; i++)
			{
				text.append( "-" );
				text.append( (this->*getItemToRemove)(md.getSelectedItems().at(i)) );
				text.append( "\n" );
			}
		}

	}
	else
	{
		// remove first
		if(numToRemove > 0)
		{
			text.append( prepareDiffHeader(rmFrom + 1, numToRemove, rmFrom, 0) );

			for(int i = 0; i < numToRemove; i++)
			{
				text += "-";
                text += (this->*getItemToRemove)(md.getSelectedItems().at(i));
                text += "\n";
			}
		}

		// insert then
		if(numToInsert > 0)
		{
			// text.append( std::string("@@ -%1,%2 +%3,%4 @@\n").arg(insTo ).arg(0).arg(insTo - numToInsert + 1).arg(numToInsert) );
			text.append( prepareDiffHeader(insTo, 0, insTo - numToRemove + 1, numToInsert) );
			for(int i = 0; i < numToInsert; i++)
			{
				text += "+";
				text += (this->*getItemToInsert)(md.getSelectedItems().at(i));
				text += "\n";
			}
		}
	}

	qDebug() << QString(text.c_str());
	CmdBase* cmd;

	enum origin commandType = md.getCommandType();

	switch(commandType)
	{
	case E_MEDIA_COL:
		cmd = new CmdEditMediaCol(m_mediaColPtr->getRevision(), text);
		break;

	case E_PLAYLIST:
		cmd = new CmdEditPlaylist(m_plPtr->getRevision(), text);
		break;

	case E_NEXTLIST:
		cmd = new CmdEditNextlist(m_nlPtr->getRevision(), text);
		break;

	default:
		qDebug() << QString("unknown command type: %1").arg(commandType);
	}

	emit sendCommand(cmd);
	// return text;
}


std::string CDiffBuilder::insertFromCollectionToCollection(comb_hash_t combhash)
{
	return ""; // m_mediaColPtr->at(pos)->asString();
}


std::string CDiffBuilder::insertFromCollectionToPlaylist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tP\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::insertFromCollectionToNextlist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tN\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::insertFromPlaylistToPlaylist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tP\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::insertFromPlaylistToNextlist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tN\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::insertFromNextlistToNextlist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tN\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::removeFromCollection(comb_hash_t combhash)
{
	return ""; //  m_mediaColPtr->at(pos)->asString();
}

std::string CDiffBuilder::removeFromPlaylist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tP\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::removeFromNextlist(comb_hash_t combhash)
{
	ostringstream oss;
	oss << "/\tN\t" << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::dummy(comb_hash_t combhash)
{
	ostringstream oss;
	oss << combhash.hash;
	return oss.str();
}

std::string CDiffBuilder::prepareDiffHeader(unsigned minusPos, unsigned minusNum, unsigned plusPos, unsigned plusNum) {
	ostringstream oss;
	oss << "@@ -" << minusPos << "," << minusNum << " +" << plusPos << "," << plusNum << " @@" << endl;

	return oss.str();
}


