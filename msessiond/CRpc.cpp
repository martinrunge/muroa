/*
 * CRpc.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CRpc.h"
#include "CConnection.h"
#include "CSessionContainer.h"
#include "CSession.h"

// command handling might move to libmuroa++
#include <cmds/Cmd.h>
#include <cmds/SimpleCmds.h>
#include <cmds/CmdError.h>
#include <cmds/CmdProgress.h>
#include <cmds/CmdFinished.h>
#include <cmds/CmdEditMediaCol.h>
#include <cmds/CmdEditPlaylist.h>
#include <cmds/CmdEditNextlist.h>


#include <vector>
#include <string>

using namespace std;

namespace muroa {

CRpc::CRpc(CConnection* connection) : m_connection(connection){
	open();
}

CRpc::~CRpc() {
	// close();
}

    void CRpc::onDataToSend(const char *data, int length) {
    	m_connection->writeData(data, length);
    }

	void CRpc::onListSessions(vector<string> sessions) {
		vector<string> knownSessions = CSessionContainer::getInstPtr()->listSessions();
		listSessions(knownSessions);
	}

	void CRpc::onJoinSession(string name) {
		m_connection->onJoinSession(name);
    }

    void CRpc::onLeaveSession()
    {
    }

    void CRpc::onPlay(uint32_t jobID)
    {
    	CmdPlay* cmd = new CmdPlay();
    	m_connection->incomingCmd(cmd);
    }

    void CRpc::onPause(uint32_t jobID)
    {
    	CmdPause* cmd = new CmdPause();
    	m_connection->incomingCmd(cmd);
    }

    void CRpc::onStop(uint32_t jobID)
    {
    	CmdStop* cmd = new CmdStop();
    	m_connection->incomingCmd(cmd);
    }

    void CRpc::onNext(uint32_t jobID)
    {
    	CmdNext* cmd = new CmdNext();
    	m_connection->incomingCmd(cmd);
    }

    void CRpc::onPrev(uint32_t jobID)
    {
    	CmdPrev* cmd = new CmdPrev();
    	m_connection->incomingCmd(cmd);
    }

    void CRpc::onStateChanged(int newState)
    {
    }

    void CRpc::onScanCollection(uint32_t jobID) {
    	// scan collection and give progress reports
    	m_connection->getSession()->scanCollection(m_connection, jobID);
    }

    void CRpc::onProgress(uint32_t jobID, int progress, int total)
    {
    }

	void CRpc::onFinished(uint32_t jobID) {
	}


    void CRpc::onError(uint32_t jobID, int errorCode, std::string description)
    {
    }

    void CRpc::onGetCollection(uint32_t jobID, unsigned  knownRev) {
    	m_connection->sendLatestMediaColRev(jobID, knownRev);
    }

    void CRpc::onGetPlaylist(uint32_t jobID, unsigned  knownRev) {
    	m_connection->sendLatestPlaylistRev(jobID, knownRev);
    }

    void CRpc::onGetNextlist(uint32_t jobID, unsigned  knownRev) {
    	m_connection->sendLatestNextlistRev(jobID, knownRev);
    }

    void CRpc::onCollection(uint32_t jobID, unsigned  diffFromRev, std::string collection) {
    }

    void CRpc::onPlaylist(uint32_t jobID, unsigned  diffFromRev, std::string playlist) {
    }

    void CRpc::onNextlist(uint32_t jobID, unsigned  diffFromRev, std::string nextlist) {
    }

    void CRpc::onEditCollection(uint32_t jobID, unsigned  fromRev, unsigned toRev, std::string collectionDiff) {
    	CmdEditMediaCol* emc = new CmdEditMediaCol(fromRev, toRev, collectionDiff);
    	m_connection->incomingCmd(emc);
    }

    void CRpc::onEditPlaylist(uint32_t jobID, unsigned  fromRev, unsigned toRev, std::string playlistDiff) {
    	CmdEditPlaylist* ep = new CmdEditPlaylist(fromRev, toRev, playlistDiff);
    	m_connection->incomingCmd(ep);
    }

    void CRpc::onEditNextlist(uint32_t jobID, unsigned  fromRev, unsigned toRev, std::string nextlistDiff) {
    	CmdEditNextlist* en = new CmdEditNextlist(fromRev, toRev, nextlistDiff);
    	m_connection->incomingCmd(en);
    }

	void CRpc::sendCmd(muroa::Cmd* cmd) {
		switch(cmd->type()) {
		case Cmd::PLAY:
		{
			// CmdPlay* msg = reinterpret_cast<CmdPlay*>(cmd);
			play(cmd->id());
			break;
		}
		case Cmd::STOP:
		{
			// CmdStop* msg = reinterpret_cast<CmdStop*>(cmd);
			stop(cmd->id());
			break;
		}
		case Cmd::NEXT:
		{
			// CmdNext* msg = reinterpret_cast<CmdNext*>(cmd);
			next(cmd->id());
			break;
		}
		case Cmd::PREV:
		{
			// CmdPrev* msg = reinterpret_cast<CmdPrev*>(cmd);
			prev(cmd->id());
			break;
		}
		case Cmd::PROGRESS:
		{
			CmdProgress* msg = reinterpret_cast<CmdProgress*>(cmd);
			progress(msg->getCorrespondingJobId(), msg->getProgress(), msg->getTotal());
			break;
		}
		case Cmd::FINISHED:
		{
			CmdFinished* msg = reinterpret_cast<CmdFinished*>(cmd);
			finished(msg->getCorrespondingJobId());
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
		case Cmd::ERROR:
		{
			CmdError* err = reinterpret_cast<CmdError*>(cmd);
			error(err->getCorrespondingJobID(), err->getErrorCode(), err->getDescription());
			break;
		}
		case Cmd::EDIT_MEDIACOL:
		{
			CmdEditMediaCol* cmd_emc = reinterpret_cast<CmdEditMediaCol*>(cmd);
			editCollection(cmd_emc->id(), cmd_emc->getFromRev(), cmd_emc->getToRev(), cmd_emc->getDiff());
			break;
		}
		case Cmd::EDIT_PLAYLIST:
		{
			CmdEditPlaylist* cmd_epl = reinterpret_cast<CmdEditPlaylist*>(cmd);
			editPlaylist(cmd_epl->id(), cmd_epl->getFromRev(), cmd_epl->getToRev(), cmd_epl->getDiff());
			break;
		}
		case Cmd::EDIT_NEXTLIST:
		{
			CmdEditNextlist* cmd_enl = reinterpret_cast<CmdEditNextlist*>(cmd);
			editNextlist(cmd_enl->id(), cmd_enl->getFromRev(), cmd_enl->getToRev(), cmd_enl->getDiff());
			break;
		}
		default:
			break;
		}
	}


} /* namespace muroa */
