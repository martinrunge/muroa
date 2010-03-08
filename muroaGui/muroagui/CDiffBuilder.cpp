/*
 * CDiffBuilder.cpp
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#include "CDiffBuilder.h"
#include "CPlaylistItem.h"

#include "CPlaylistCommand.h"


CDiffBuilder::CDiffBuilder(CCollection<CCollectionItem*>* colPtr,
						   CCollection<CPlaylistItem*>* plPtr,
						   CCollection<CPlaylistItem*>* nlPtr ):
	m_collectionPtr(colPtr) ,
	m_playlistPtr(plPtr) ,
	m_nextlistPtr(nlPtr)
{

}

CDiffBuilder::~CDiffBuilder() {

}


void CDiffBuilder::prepareDiff(CModelDiff* md)
{
	enum origin dest = md->getDestination();
	enum origin orig = md->getOrigin();

	int numToRemove = 0;
	int numToInsert = 0;

	switch(orig)
	{
	case E_COLLECTION:
		this->getItemToRemove = &CDiffBuilder::removeFromCollection;
		switch(dest)
		{
		case E_COLLECTION:
			// move inside collection
			numToRemove = md->getNumSelected();
			numToInsert = numToRemove;
			getItemToInsert = &CDiffBuilder::insertFromCollectionToCollection;

		case E_PLAYLIST:
			// insert songs from collection into playlist
			numToInsert = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::insertFromCollectionToPlaylist;

			break;

		case E_NEXTLIST:
			// two possible modes:
			// insert songs from collection into nextlist AND append to playlist
			// insert songs into nextlist only, so they disappear after playing
			numToInsert = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::insertFromCollectionToPlaylist;  // playlist is correct here as only a string with the hash from collection is returned
			break;

		default:
			qDebug() << QString("CDiffBuilder::getText: unknown destination: %1").arg(dest);
		}
		break;

	case E_PLAYLIST:
		getItemToRemove = &CDiffBuilder::removeFromPlaylist;
		switch(dest)
		{
		case E_COLLECTION:
			// remove songs from playlist
			numToRemove = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::dummy;
			break;

		case E_PLAYLIST:
			// move songs inside playlist
			numToRemove = md->getNumSelected();
			numToInsert = numToRemove;
			getItemToInsert = &CDiffBuilder::insertFromPlaylist;
			break;

		case E_NEXTLIST:
			// insert songs from playlist into nextlist at drop position
			numToInsert = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::insertFromPlaylist;  // playlist is same as nectlist in this case
			break;

		default:
			qDebug() << QString("CDiffBuilder::getText: unknown destination: %1").arg(dest);
		}
		break;

	case E_NEXTLIST:
		getItemToRemove = &CDiffBuilder::removeFromNextlist;
		switch(dest)
		{
		case E_COLLECTION:
		case E_PLAYLIST:
			// remove selected songs from nextlist
			numToRemove = md->getNumSelected();
			getItemToInsert = &CDiffBuilder::dummy;
			break;

		case E_NEXTLIST:
			// move selected songs inside nextlist
			numToInsert = md->getNumSelected();
			numToRemove = numToInsert;
			getItemToInsert = &CDiffBuilder::insertFromNextlist;
			break;

		default:
			qDebug() << QString("CDiffBuilder::getText: unknown destination: %1").arg(dest);
		}
		break;

	default:
		qDebug() << QString("CDiffBuilder::getText: unknown origin: %1").arg(orig);
	}

	md->setNumToInsert(numToInsert);
	md->setNumToRemove(numToRemove);

}

QString CDiffBuilder::diff(CModelDiff md)
{
	QString text;

	prepareDiff(&md);

	int numToInsert = md.getNumToInsert();
	int numToRemove = md.getNumToRemove();

	int rmFrom = md.getSelectedIndexes().at(0);
	int rmTo = md.getSelectedIndexes().at( md.getNumSelected() - 1 );

	int insTo = md.getInsertPos();

	// ignore, if rows to remove are to be moved into their own range. (dropped on self)
	if( rmFrom < insTo && rmTo > insTo )
	{
		return text;
	}

	if(rmFrom > insTo)
	{
		// insert first
		if(numToInsert > 0)
		{
			text.append( QString("@@ -%1,%2 +%3,%4 @@\n").arg(insTo).arg(0).arg(insTo +1).arg(numToInsert) );
			for(int i = 0; i < numToInsert; i++)
			{
				int row = md.getSelectedIndexes().at(i);
				text.append( QString("+%1\n").arg( (this->*getItemToInsert)(row) ) );
			}
		}

		// remove then
		if(numToRemove > 0)
		{
			text.append( QString("@@ -%1,%2 +%3,%4 @@\n").arg(rmFrom + 1).arg(numToRemove).arg(rmFrom + numToRemove).arg(0) );
			for(int i = 0; i < numToRemove; i++)
			{
				int row = md.getSelectedIndexes().at(i);
				text.append( QString("-%1\n").arg( (this->*getItemToRemove)(row) ) );
			}
		}

	}
	else
	{
		// remove first
		if(numToRemove > 0)
		{
			text.append( QString("@@ -%1,%2 +%3,%4 @@\n").arg(rmFrom + 1).arg(numToRemove).arg(rmFrom).arg(0) );
			for(int i = 0; i < numToRemove; i++)
			{
				int row = md.getSelectedIndexes().at(i);
				text.append( QString("-%1\n").arg( (this->*getItemToRemove)(row) ) );
			}
		}

		// insert then
		if(numToInsert > 0)
		{
			text.append( QString("@@ -%1,%2 +%3,%4 @@\n").arg(insTo ).arg(0).arg(insTo - numToInsert + 1).arg(numToInsert) );
			for(int i = 0; i < numToInsert; i++)
			{
				int row = md.getSelectedIndexes().at(i);
				text.append( QString("+%1\n").arg( (this->*getItemToInsert)(row) ) );
			}
		}
	}

	qDebug() << text;

	CPlaylistCommand plCmd(text);
	emit sendCommand(plCmd);

	return text;
}


QString CDiffBuilder::insertFromCollectionToCollection(int pos)
{
	return m_collectionPtr->at(pos)->asString();
}


QString CDiffBuilder::insertFromCollectionToPlaylist(int pos)
{
	return QString::number(m_collectionPtr->at(pos)->getHash());
}

QString CDiffBuilder::insertFromPlaylist(int pos)
{
	return m_playlistPtr->at(pos)->asString();
}

QString CDiffBuilder::insertFromNextlist(int pos)
{
	return m_nextlistPtr->at(pos)->asString();
}

QString CDiffBuilder::removeFromCollection(int pos)
{
	return m_collectionPtr->at(pos)->asString();
}

QString CDiffBuilder::removeFromPlaylist(int pos)
{
	return m_playlistPtr->at(pos)->asString();
}

QString CDiffBuilder::removeFromNextlist(int pos)
{
	return m_nextlistPtr->at(pos)->asString();
}

QString CDiffBuilder::dummy(int pos)
{
	return QString();
}



