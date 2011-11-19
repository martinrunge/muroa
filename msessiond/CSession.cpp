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

CSession::CSession(string name) : m_name(name){
	// TODO Auto-generated constructor stub

}

CSession::~CSession() {
	// TODO Auto-generated destructor stub
}

void CSession::addConnection(CConnection::pointer ptr) {
	m_connections.insert(ptr);
}

void CSession::removeConnection(CConnection::pointer ptr) {
	m_connections.erase(ptr);
}

void CSession::toAll( CCmdBase* cmd ) {

}


} /* namespace muroa */
