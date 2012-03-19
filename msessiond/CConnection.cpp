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
		m_rpc->error(0, 0, "cannot join dummy session "CREATE_NEW_SESSION);
	}
	else {
		setSession( m_session_container->assignConnection(this, name) );
	}
}

void CConnection::joinSession(std::string name) {
	m_rpc->joinSession(name);
}

void CConnection::sendLatestMediaColRev(unsigned knownRev) {
	std::string diff;
	try {
		if(knownRev == 0) {
			CRootItem* ri = m_session->getMediaCol();  // get latest rev
			diff = ri->serialize();
		}
		else {
			diff = m_session->getMediaColDiff(knownRev);
		}
		m_rpc->editCollection(knownRev, diff);
	}
	catch(InvalidMsgException iex) {
		m_rpc->error(0, 0, iex.what());
	}
}

void CConnection::sendLatestPlaylistRev(unsigned knownRev) {
	std::string diff;
	try {
		if(knownRev == 0) {
			CRootItem* ri = m_session->getPlaylist();  // get latest rev
			diff = ri->serialize();
		}
		else {
			diff = m_session->getPlaylistDiff(knownRev);
		}
		m_rpc->editPlaylist(knownRev, diff);
	}
	catch(InvalidMsgException iex) {
		m_rpc->error(0, 0, iex.what());
	}
}

void CConnection::sendLatestNextlistRev(unsigned knownRev) {
	std::string diff;
	try {
		if(knownRev == 0) {
			CRootItem* ri = m_session->getNextlist();  // get latest rev
			diff = ri->serialize();
		}
		else {
			diff = m_session->getNextlistDiff(knownRev);
		}
		m_rpc->editNextlist(knownRev, diff);
	}
	catch(InvalidMsgException iex) {
		m_rpc->error(0, 0, iex.what());
	}
}


CSession *CConnection::getSession() const {
	return m_session;
}

void CConnection::setSession(CSession *session) {
	m_session = session;
}

void CConnection::incomingCmd( Cmd* cmd ) {
	m_session->incomingCmd(cmd, this);
}

void CConnection::sendCmd( Cmd* cmd ) {
	m_rpc->sendCmd(cmd);
}

void CConnection::play(CmdPlay* playCmd) {
	m_rpc->play();
}

void CConnection::stop(CmdStop* stopCmd) {
	m_rpc->stop();
}

void CConnection::next(CmdNext* nextCmd) {
	m_rpc->next();
}

void CConnection::prev(CmdPrev* prevCmd) {
	m_rpc->prev();
}

void CConnection::progress(CmdProgress* progressMsg) {
	m_rpc->progress(progressMsg->getCorrespondingJobId(), progressMsg->getProgress());
}

void CConnection::finished(CmdFinished* finishedCmd) {
	m_rpc->finished(finishedCmd->getCorrespondingJobId());
}

void CConnection::collectionChanged(CmdEditMediaCol* colChangedCmd) {
	m_rpc->editCollection(colChangedCmd->getFromRev(), colChangedCmd->getDiff());
}

void CConnection::response(CmdResp* respCmd) {
//	m_rpc->
}

void CConnection::error(CmdError* errorMsg) {
	m_rpc->error(errorMsg->getCorrespondingJobID(), errorMsg->getErrorCode(), errorMsg->getDescription());
}

} /* namespace muroa */
