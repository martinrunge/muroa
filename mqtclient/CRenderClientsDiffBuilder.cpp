/*
 * CRenderClientsDiffBuilder.cpp
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#include "CRenderClientsDiffBuilder.h"
#include "CPlaylistItem.h"
#include "CStreamClientItem.h"

#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"
#include "cmds/CmdEditSessionState.h"

using namespace muroa;

CRenderClientsDiffBuilder::CRenderClientsDiffBuilder( CRootItem* sstPtr) : m_sessionStatePtr(sstPtr),
		                                                                   m_render_clients(0)
{
}

CRenderClientsDiffBuilder::~CRenderClientsDiffBuilder() {

}

void CRenderClientsDiffBuilder::prepareDiff(CModelDiff* md) {
	enum origin dest = md->getDestination();
	enum origin orig = md->getOrigin();

	enum origin commandType;

	m_render_clients = m_sessionStatePtr->getCategoryPtr("/RenderClients");

	int numToRemove = 0;
	int numToInsert = 0;

	md->sort();

	switch (orig) {
	case E_AVAIL_RENDER_CLIENT:
		commandType = E_AVAIL_RENDER_CLIENT;
		numToRemove = 0;
		numToInsert = md->getNumSelected();
	    getItemToRemove = &CRenderClientsDiffBuilder::removeFromOwnList;
		getItemToInsert = &CRenderClientsDiffBuilder::insertInOwnList;
		break;

	case E_OWN_RENDER_CLIENT:
		commandType = E_OWN_RENDER_CLIENT;
		numToRemove = md->getNumSelected();
		numToInsert = 0;
	    getItemToRemove = &CRenderClientsDiffBuilder::removeFromOwnList;
	    getItemToInsert = &CRenderClientsDiffBuilder::insertInAvailList;
		break;


	default:
		qDebug()
				<< QString("CDiffBuilder::getText: unknown origin: %1").arg(
						orig);
	}

	md->setCommandType(commandType);
	md->setNumToInsert(numToInsert);
	md->setNumToRemove(numToRemove);
}

std::string CRenderClientsDiffBuilder::diff(CModelDiff md) {
	std::string text;

	prepareDiff(&md);

	int numToInsert = md.getNumToInsert();
	int numToRemove = md.getNumToRemove();

	int rmFrom = md.getSelectedItems().at(0).line;
	int rmTo = md.getSelectedItems().at(md.getNumSelected() - 1).line;

	int insTo = md.getInsertPos();

	// ignore, if rows to remove are to be moved into their own range. (dropped on self)
	if (rmFrom < insTo && rmTo > insTo
			&& md.getOrigin() == md.getDestination()) {
		//qDebug() << std::string("dropped on origin -> ignore: [%1, %2] -> %3").arg(rmFrom).arg(rmTo).arg(insTo);
		//qDebug() << std::string("from: %1 to: %2 ").arg(md.getOrigin()).arg(md.getDestination());
		//md.dump();
		return text;
	} else {
		//qDebug() << std::string("building diff: [%1, %2] -> %3").arg(rmFrom).arg(rmTo).arg(insTo);
		//md.dump();
	}

	if (rmFrom > insTo) {
		// insert first
		if (numToInsert > 0) {
			text.append(prepareDiffHeader(insTo, 0, insTo + 1, numToInsert));
			for (int i = 0; i < numToInsert; i++) {
				text.append("+");
				text.append( (this->*getItemToInsert)(md.getSelectedItems().at(i)));
				text.append("\n");
			}
		}

		// remove then
		if (numToRemove > 0) {
			text.append(
					prepareDiffHeader(rmFrom + 1, numToRemove,
							rmFrom + numToInsert, 0));
			for (int i = 0; i < numToRemove; i++) {
				text.append("-");
				text.append( (this->*getItemToRemove)(md.getSelectedItems().at(i)));
				text.append("\n");
			}
		}

	} else {
		// remove first
		if (numToRemove > 0) {
			text.append(prepareDiffHeader(rmFrom + 1, numToRemove, rmFrom, 0));

			for (int i = 0; i < numToRemove; i++) {
				text += "-";
				text += (this->*getItemToRemove)(md.getSelectedItems().at(i));
				text += "\n";
			}
		}

		// insert then
		if (numToInsert > 0) {
			// text.append( std::string("@@ -%1,%2 +%3,%4 @@\n").arg(insTo ).arg(0).arg(insTo - numToInsert + 1).arg(numToInsert) );
			text.append(
					prepareDiffHeader(insTo, 0, insTo - numToRemove + 1,
							numToInsert));
			for (int i = 0; i < numToInsert; i++) {
				text += "+";
				text += (this->*getItemToInsert)(md.getSelectedItems().at(i));
				text += "\n";
			}
		}
	}

	qDebug() << QString(text.c_str());
	CmdBase* cmd;

	enum origin commandType = md.getCommandType();

	switch (commandType) {
	case E_AVAIL_RENDER_CLIENT:
	case E_OWN_RENDER_CLIENT:
		cmd = new CmdEditSessionState(m_sessionStatePtr->getRevision(), text);
		break;

	default:
		qDebug() << QString("unknown command type: %1").arg(commandType);
	}

	emit sendCommand(cmd);
	// return text;
}

std::string CRenderClientsDiffBuilder::insertInOwnList(comb_hash_t comb_hash) {
	muroa::IContentItem* citem = m_render_clients->getContentItem(comb_hash.line);
	if(citem->type() == CItemType::E_STREAM_CLIENT) {
		CStreamClientItem* scitem = reinterpret_cast<CStreamClientItem*>(citem);
		string result;
		result.reserve(m_render_clients->getPath().size() + scitem->getText().size() + 2 );
		result = m_render_clients->getPath();
		result.append("\t");
		result.append( scitem->getText() );
		return result;
	}
	else return "";
}

std::string CRenderClientsDiffBuilder::insertInAvailList(comb_hash_t comb_hash) {
	muroa::IContentItem* citem = m_render_clients->getContentItem(comb_hash.line);
	if(citem->type() == CItemType::E_STREAM_CLIENT) {
		CStreamClientItem* scitem = reinterpret_cast<CStreamClientItem*>(citem);
		string result;
		result.reserve(m_render_clients->getPath().size() + scitem->getText().size() + 2 );
		result = m_render_clients->getPath();
		result.append("\t");
		result.append( scitem->getText() );
		return result;
	}
	else return "";
}

std::string CRenderClientsDiffBuilder::removeFromOwnList(comb_hash_t comb_hash) {
	muroa::IContentItem* citem = m_render_clients->getContentItem(comb_hash.line);
	if(citem->type() == CItemType::E_STREAM_CLIENT) {
		CStreamClientItem* scitem = reinterpret_cast<CStreamClientItem*>(citem);
		string result;
		result.reserve(m_render_clients->getPath().size() + scitem->getText().size() + 2 );
		result = m_render_clients->getPath();
		result.append("\t");
		result.append( scitem->getText() );
		return result;
	}
	else return "";
}

std::string CRenderClientsDiffBuilder::removeFromAvailList(comb_hash_t comb_hash) {
	muroa::IContentItem* citem = m_render_clients->getContentItem(comb_hash.line);
	if(citem->type() == CItemType::E_STREAM_CLIENT) {
		CStreamClientItem* scitem = reinterpret_cast<CStreamClientItem*>(citem);
		string result;
		result.reserve(m_render_clients->getPath().size() + scitem->getText().size() + 2 );
		result = m_render_clients->getPath();
		result.append("\t");
		result.append( scitem->getText() );
		return result;
	}
	else return "";
}

std::string CRenderClientsDiffBuilder::dummy(comb_hash_t combhash) {
	ostringstream oss;
	oss << combhash.hash;
	return oss.str();
}

std::string CRenderClientsDiffBuilder::prepareDiffHeader(unsigned minusPos,
		                                                 unsigned minusNum,
		                                                 unsigned plusPos,
		                                                 unsigned plusNum)
{
	ostringstream oss;
	oss << "@@ -" << minusPos << "," << minusNum << " +" << plusPos << ","
			<< plusNum << " @@" << endl;

	return oss.str();
}

