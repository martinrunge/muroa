/*
 * CCmdDispatcher.cpp
 *
 *  Created on: 19 Mar 2012
 *      Author: martin
 */

#include "CCmdDispatcher.h"
#include "CSession.h"

#include <cmds/Cmd.h>
#include <cmds/SimpleCmds.h>
#include <cmds/CmdError.h>
#include <cmds/SimpleCmds.h>
#include <cmds/CmdEditMediaCol.h>
#include <cmds/CmdEditPlaylist.h>
#include <cmds/CmdEditNextlist.h>

#include <CPlaylistItem.h>
#include <CNextlistItem.h>

#include <CUtils.h>

#include <boost/regex.hpp>

using namespace muroa;

CCmdDispatcher::CCmdDispatcher(muroa::CSession* session) : m_session(session) {
}

CCmdDispatcher::~CCmdDispatcher() {
}

void CCmdDispatcher::incomingCmd(muroa::Cmd* cmd) {
	switch(cmd->type()) {

	case Cmd::PLAY:
	{
		m_session->play();
		break;
	}
	case Cmd::PAUSE:
	{
		m_session->pause();
		break;
	}
	case Cmd::STOP:
	{
		m_session->stop();
		break;
	}
	case Cmd::NEXT:
	{
		m_session->next();
		break;
	}
	case Cmd::PREV:
	{
		m_session->prev();
		break;
	}
	case Cmd::ERROR:
	{
		break;
	}
	case Cmd::FINISHED:
	{
		break;
	}
	case Cmd::PROGRESS:
	{
		break;
	}
	case Cmd::OPENDB:
	{
		break;
	}
	case Cmd::RESP:
	{
		break;
	}
	case Cmd::EDIT_MEDIACOL:
	{
		CmdEditMediaCol* cmd_em = reinterpret_cast<CmdEditMediaCol*>(cmd);
		try {
			m_session->addMediaColRevFromDiff(cmd_em->getDiff(), cmd_em->getFromRev());
			// if there was no exception, the diff is ok, send it to all clients
			m_session->toAll(cmd_em);
		}
		catch(ExMalformedPatch& ex) {
			CmdError* errMsg = new CmdError(cmd_em->id(), 0, ex.reason());
			m_session->sendToInitiator(errMsg, cmd_em->getConnectionID() );
		}
		break;
	}
	case Cmd::EDIT_PLAYLIST:
	{
		CmdEditPlaylist* cmd_epl = reinterpret_cast<CmdEditPlaylist*>(cmd);
		try {
			int current_playlist_rev = m_session->getMaxPlaylistRev();
			m_session->addPlaylistRevFromDiff(cmd_epl->getDiff(), cmd_epl->getFromRev());
			// if there was no exception, the diff is ok, send it to all clients

			cmd_epl->setToRev( m_session->getMaxPlaylistRev() );
			if( current_playlist_rev == 0 ) {
				cmd_epl->setDiff(m_session->getPlaylist()->serialize());
			}
			else {
				addIdToPlaylistDiff(cmd_epl);
			}
			m_session->toAll(cmd_epl);
		}
		catch(ExMalformedPatch& ex) {
			CmdError* errMsg = new CmdError(cmd_epl->id(), 0, ex.reason());
			m_session->sendToInitiator(errMsg, cmd_epl->getConnectionID() );
		}
		break;
	}
	case Cmd::EDIT_NEXTLIST:
	{
		CmdEditNextlist* cmd_enl = reinterpret_cast<CmdEditNextlist*>(cmd);
		try {
			int current_nextlist_rev = m_session->getMaxNextlistRev();
			m_session->addNextlistRevFromDiff(cmd_enl->getDiff(), cmd_enl->getFromRev());
			// if there was no exception, the diff is ok, send it to all clients
			if( current_nextlist_rev == 0 ) {
				cmd_enl->setDiff(m_session->getNextlist()->serialize());
			}
			cmd_enl->setToRev( m_session->getMaxNextlistRev() );
			m_session->toAll(cmd_enl);
		}
		catch(ExMalformedPatch& ex) {
			CmdError* errMsg = new CmdError(cmd_enl->id(), 0, ex.reason());
			m_session->sendToInitiator(errMsg, cmd_enl->getConnectionID() );
		}
		break;
	}
	default:

		break;
	}
}

void CCmdDispatcher::addIdToPlaylistDiff(muroa::CmdEditPlaylist* epl_cmd) throw(ExMalformedPatch) {
	string diff = epl_cmd->getDiff();
	string output;
	output.reserve(2 * diff.size());

	istringstream iss(diff);

	CCategoryItem* parent = 0;

	bool new_category = true;
    boost::regex rx("^@@ -(\\d+),(\\d+)\\s+\\+(\\d+),(\\d+)\\s*@@$");
    int oldStart(0);
	int oldLen(0);
	int newStart(0);
	int newLen(0);

	int lineNr = 0;
	int patchLineNr = 0;
	int chunkSizeSum = 0;

	string line;

	while(!iss.eof()) {
		getline(iss, line);
		if(iss.bad()) {
			cerr << "CRootItem::patch: Error reading lines." << endl;
		}
		patchLineNr++;

	    if( line.find("@@") == 0 ) {
			// diff chunk header
			boost::cmatch res;
			boost::regex_search(line.c_str(), res, rx);

			string oldStartStr = res[1];
			string oldLenStr = res[2];
			string newStartStr = res[3];
			string newLenStr = res[4];

			try
			{
				oldStart = CUtils::str2long( oldStartStr );
				oldLen = CUtils::str2long( oldLenStr );
				newStart = CUtils::str2long( newStartStr );
				newLen = CUtils::str2long( newLenStr );
			}
			catch(std::invalid_argument ex)
			{
				throw ExMalformedPatch(ex.what(), lineNr);
			}

			if(oldLen == 0) oldStart++;
			lineNr = oldStart + chunkSizeSum;

			chunkSizeSum += newLen - oldLen;
			output += line + "\n";

		}
		else if( line.size() > 0 ) {
			char sign = line[0];
			string content = line.substr(1, line.size() - 1);

			switch(sign){
				case '+': //only new lines need attention here.
				{
					CRootItem* ri = m_session->getPlaylist();
					CPlaylistItem* newItem = reinterpret_cast<CPlaylistItem*>(ri->getBase()->getContentItem(lineNr - 1));
					output += "+/\t";
					output += newItem->serialize();
					break;
				}
				default:
					output += line + "\n";
					break;
			}
			lineNr++;
		}
	}

	epl_cmd->setDiff(output);
}
