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

namespace muroa {

CConnection::CConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service) , m_rpc(0) {
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

void CConnection::joinSession(std::string name) {
	if(name.compare(CREATE_NEW_SESSION) == 0) {
		m_rpc->error(0, 0, "cannot join dummy session "CREATE_NEW_SESSION);
	}
	else {
		setSession( m_session_container->assignConnection(this, name) );
	}
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


} /* namespace muroa */
