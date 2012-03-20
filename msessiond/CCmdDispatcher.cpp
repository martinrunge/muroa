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


using namespace muroa;

CCmdDispatcher::CCmdDispatcher(muroa::CSession* session) : m_session(session) {
}

CCmdDispatcher::~CCmdDispatcher() {
}

void CCmdDispatcher::incomingCmd(muroa::Cmd* cmd) {
	switch(cmd->type()) {

	case Cmd::PLAY:
	{
		break;
	}
	case Cmd::STOP:
	{
		break;
	}
	case Cmd::NEXT:
	{
		break;
	}
	case Cmd::PREV:
	{
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
		catch(MalformedPatchEx ex) {
			CmdError* errMsg = new CmdError(cmd_em->id(), 0, ex.getReason());
			m_session->sendToInitiator(errMsg, cmd_em->getConnectionID() );
		}
		break;
	}
	case Cmd::EDIT_PLAYLIST:
	{
		CmdEditPlaylist* cmd_epl = reinterpret_cast<CmdEditPlaylist*>(cmd);
		try {
			m_session->addPlaylistRevFromDiff(cmd_epl->getDiff(), cmd_epl->getFromRev());
			// if there was no exception, the diff is ok, send it to all clients
			m_session->toAll(cmd_epl);
		}
		catch(MalformedPatchEx ex) {
			CmdError* errMsg = new CmdError(cmd_epl->id(), 0, ex.getReason());
			m_session->sendToInitiator(errMsg, cmd_epl->getConnectionID() );
		}
		break;
	}
	case Cmd::EDIT_NEXTLIST:
	{
		break;
	}
	default:

		break;
	}
}
