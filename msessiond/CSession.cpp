/*
 * CSession.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CSession.h"

#include <cmds/CCmdBase.h>

#include <CTcpServer.h>

using namespace std;

namespace muroa {

CSession::CSession() {
	// TODO Auto-generated constructor stub

}

CSession::~CSession() {
	// TODO Auto-generated destructor stub
}

void CSession::toAll( CCmdBase* cmd ) {

}

std::set<CTcpConnection::pointer> CSession::getConnections() {
	m_tcp_server->getConnctionManager()->getConnections();
}


} /* namespace muroa */
