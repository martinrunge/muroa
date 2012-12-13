/*
 * CRenderClientsDiffBuilder.cpp
 *
 *  Created on: 16 Feb 2010
 *      Author: martin
 */

#include "CRenderClientsDiffBuilder.h"
#include "CPlaylistItem.h"
#include "CStreamClientItem.h"
#include "CSession.h"

#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"
#include "cmds/CmdEditSessionState.h"

using namespace muroa;

CRenderClientsDiffBuilder::CRenderClientsDiffBuilder( CRootItem* sstPtr, const CSession* const session) :
                                                               m_sessionStatePtr(sstPtr),
                                                               m_session(session),
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
    if(m_render_clients == 0) {
        m_render_clients = m_sessionStatePtr->mkPath("/renderClients");
    }

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

	int numToMod = md.getNumToInsert();

	int modFrom = md.getSelectedItems().at(0).line;
	int modTo   = md.getSelectedItems().at(md.getNumSelected() - 1).line;

	int insTo = md.getInsertPos();

	// in /RenderClients Category, indices are always the same as no entry is actually inserted or removed
	// we just modify the "ownerSessionName" property
	if(md.getOrigin() == E_AVAIL_RENDER_CLIENT )
	{
	    if(md.getDestination() == E_OWN_RENDER_CLIENT ) {
	        // avail -> own: make streaming clients part of own session
	        text = prepareDiffHeader(modFrom + 1, numToMod, modFrom + 1, numToMod);
            for(int i = modFrom; i <= modTo; i++) {
	            IContentItem* ci = m_render_clients->getContentItem( i );
	            CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
	            assert(sci != 0);
	            if( m_session->getName().compare(sci->getOwnerSessionName()) != 0 ) {
	                text += "-";
                    text += m_render_clients->getPath();
                    text += "\t";
	                text += sci->serialize(false);
	            }
	        }

	        for(int i = modFrom; i <= modTo; i++) {
                IContentItem* ci = m_render_clients->getContentItem( i );
                CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
                assert(sci != 0);
                if( m_session->getName().compare(sci->getOwnerSessionName()) != 0 ) {
                    sci->setOwnerSessionName(m_session->getName());
                    text += "+";
                    text += m_render_clients->getPath();
                    text += "\t";
                    text += sci->serialize(false);
                }
            }

	    }
	    else {
	        return string();
	    }
	}
	else {
	    if(md.getDestination() == E_AVAIL_RENDER_CLIENT ) {
	        // own -> avail: remove clients from own session
            text = prepareDiffHeader(modFrom, numToMod, modTo, numToMod);
            for(int i = modFrom; i <= modTo; i++) {
                IContentItem* ci = m_render_clients->getContentItem( i );
                CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
                assert(sci != 0);
                if( m_session->getName().compare(sci->getOwnerSessionName()) == 0 ) {
                    text += "-";
                    text += m_render_clients->getPath();
                    text += "\t";
                    text += sci->serialize(false);
                }
            }

            for(int i = modFrom; i <= modTo; i++) {
                IContentItem* ci = m_render_clients->getContentItem( i );
                CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
                assert(sci != 0);
                if( m_session->getName().compare(sci->getOwnerSessionName()) == 0 ) {
                    sci->setOwnerSessionName("");
                    text += "+";
                    text += m_render_clients->getPath();
                    text += "\t";
                    text += sci->serialize(false);
                }
            }

	    }
	    else {
	        return string();
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
	return text;
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

