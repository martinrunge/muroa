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
			m_session->addMediaColRevFromDiff(cmd_em->getDiff(), cmd_em->m_fromRev);
		}
		catch(InvalidMsgException ex) {
			CmdError* errMsg = new CmdError(cmd_em->id(), 0, ex.reason());
			m_session->sendToInitiator(cmd_em->id(), errMsg );
		}
		break;
	}
	case Cmd::EDIT_PLAYLIST:
	{
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
