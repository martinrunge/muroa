/*
 * CConnection.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CSessionContainer.h"
#include "muroaConstants.h"

#include "CConnection.h"
#include "CRpc.h"

namespace muroa {

CConnection::CConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service) {
	m_session_container = CSessionContainer::getInstPtr();
	m_rpc = new CRpc(this);

	m_session_container->addSessionlessConnection(boost::enable_shared_from_this<CConnection>::shared_from_this());
}

CConnection::~CConnection() {
	delete m_rpc;
}

void CConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	m_rpc->newData(buffer.data(), length);
}

void CConnection::joinSession(std::string name) {
	if(name.compare(CREATE_NEW_SESSION) == 0) {
		m_rpc->error(0, 0, "cannot join dummy session "CREATE_NEW_SESSION);
	}
	else {
		setSession( m_session_container->assignConnection(boost::enable_shared_from_this<CConnection>::shared_from_this(), name) );
	}
}


CSession *CConnection::getSession() const {
	return m_session;
}

void CConnection::setSession(CSession *session) {
	m_session = session;
}


} /* namespace muroa */
