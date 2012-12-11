/*
 * CConnection.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CSessionContainer.h"
#include "muroaConstants.h"

#include "CRootItem.h"
#include "CSession.h"
#include "CConnection.h"
#include "CRpc.h"

#include <cmds/Cmd.h>
#include <cmds/SimpleCmds.h>
#include <cmds/CmdError.h>
#include <cmds/CmdProgress.h>
#include <cmds/CmdFinished.h>
#include <cmds/CmdEditMediaCol.h>

namespace muroa {

unsigned CConnection::m_next_id(0);

CConnection::CConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service) , m_rpc(0), m_id(m_next_id++) {
	m_session_container = CSessionContainer::getInstPtr();
}

CConnection::~CConnection() {
	if(m_rpc != 0) delete m_rpc;
}

void CConnection::start() {
	CTcpConnection::start();
	m_rpc = new CRpc(this);
}

void CConnection::stop() {
	if(m_rpc != 0) {
		delete m_rpc;
		m_rpc = 0;
	}

	CTcpConnection::stop();
}

void CConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	m_rpc->newData(buffer.data(), length);
}

void CConnection::onJoinSession(std::string name) {
	if(name.compare(CREATE_NEW_SESSION) == 0) {
		m_rpc->error(0, 0, "cannot join dummy session "\
				            CREATE_NEW_SESSION);
	}
	else {
		setSession( m_session_container->assignConnection(this, name) );
	}
}

void CConnection::joinSession(std::string name) {
	m_rpc->joinSession(name);
}

void CConnection::sendLatestMediaColRev(uint32_t jobID, unsigned knownRev) {
	std::string diff;
	try {
		if( knownRev > 0 && m_session->hasMediaColRev( knownRev ))
		{
			diff = m_session->getMediaColDiff(knownRev);
		}
		else
		{
			CRootItem* ri = m_session->getMediaCol();  // get latest rev
			diff = ri->serialize();
		}
		m_rpc->editCollection( jobID, knownRev, m_session->getMaxMediaColRev(), diff);
	}
	catch(ExInvMsg& iex) {
		m_rpc->error(jobID, 0, iex.what());
	}
}

void CConnection::sendLatestPlaylistRev(uint32_t jobID, unsigned knownRev) {
	std::string diff;
	try {
		if( knownRev > 0 && m_session->hasPlaylistRev( knownRev))
		{
            diff = m_session->getPlaylistDiff(knownRev);
		}
		else
		{
            CRootItem* ri = m_session->getPlaylist();  // get latest rev
            diff = ri->serialize();
		}
		m_rpc->editPlaylist(jobID, knownRev, m_session->getMaxPlaylistRev(), diff);
	}
	catch(ExInvMsg& iex) {
		m_rpc->error(jobID, 0, iex.what());
	}
}

void CConnection::sendLatestNextlistRev(uint32_t jobID, unsigned knownRev) {
	std::string diff;
	try {
		if( knownRev > 0 && m_session->hasNextlistRev( knownRev )) {
            diff = m_session->getNextlistDiff(knownRev);
		}
		else
		{
            CRootItem* ri = m_session->getNextlist();  // get latest rev
            diff = ri->serialize();
		}
		m_rpc->editNextlist(jobID, knownRev, m_session->getMaxNextlistRev(), diff);
	}
	catch(ExInvMsg& iex) {
		m_rpc->error(jobID, 0, iex.what());
	}
}

void CConnection::sendLatestSessionStateRev(uint32_t jobID, unsigned knownRev) {
	std::string diff;
	try {
		if( knownRev > 0 && m_session->hasSessionStateRev( knownRev )) {
            diff = m_session->getSessionStateDiff(knownRev);
		}
		else
		{
            CRootItem* ri = m_session->getSessionState();  // get latest rev
            diff = ri->serialize();
		}
		m_rpc->editSessionState(jobID, knownRev, m_session->getMaxSessionStateRev(), diff);
	}
	catch(ExInvMsg& iex) {
		m_rpc->error(jobID, 0, iex.what());
	}
}


CSession *CConnection::getSession() const {
	return m_session;
}

void CConnection::setSession(CSession *session) {
	m_session = session;
}

void CConnection::incomingCmd( Cmd* cmd ) {
	cmd->setConnectionID(m_id);
	try {
		m_session->incomingCmd(cmd, this);
	}
	catch(ExInvMsg& invmex)
	{
		CmdError *cmdErr = new CmdError(cmd->id(), 0, invmex.what());
		error(cmdErr);
	}
}

void CConnection::sendCmd( Cmd* cmd ) {
	m_rpc->sendCmd(cmd);
}

void CConnection::play(CmdPlay* playCmd) {
	m_rpc->play(playCmd->id());
}

void CConnection::stop(CmdStop* stopCmd) {
	m_rpc->stop(stopCmd->id());
}

void CConnection::next(CmdNext* nextCmd) {
	m_rpc->next(nextCmd->id());
}

void CConnection::prev(CmdPrev* prevCmd) {
	m_rpc->prev(prevCmd->id());
}

void CConnection::progress(CmdProgress* progressMsg) {
	m_rpc->progress(progressMsg->getCorrespondingJobId(), progressMsg->getProgress(), progressMsg->getTotal());
}

void CConnection::finished(CmdFinished* finishedCmd) {
	m_rpc->finished(finishedCmd->getCorrespondingJobId());
}

void CConnection::collectionChanged(CmdEditMediaCol* colChangedCmd) {
	m_rpc->editCollection(colChangedCmd->id(), colChangedCmd->getFromRev(), colChangedCmd->getToRev(), colChangedCmd->getDiff());
}

void CConnection::response(CmdResp* respCmd) {
//	m_rpc->
}

void CConnection::error(CmdError* errorMsg) {
	m_rpc->error(errorMsg->getCorrespondingJobID(), errorMsg->getErrorCode(), errorMsg->getDescription());
}

void CConnection::onClose() {
	if(m_session !=0) {
		m_session->removeConnection(this);
	}
	else {
		assert(m_session_container != 0);
		m_session_container->remove(this);
	}
}


} /* namespace muroa */
