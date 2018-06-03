/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "CStreamServer.h"
#include "CStreamCtrlConnection.h"
#include "CStreamConnection.h"
#include "Exceptions.h"
#include "cmds/CmdStreamReset.h"

#include <CApp.h>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>


using namespace std;
using namespace boost::posix_time;
using namespace muroa;
using namespace log4cplus;

namespace muroa {

CStreamServer::CStreamServer(boost::asio::io_service& io_service, std::string session_name, int timeServerPort, int session_id, int transport_buffer_size_in_ms) :
                             CMediaStreamProvider(io_service, session_id, transport_buffer_size_in_ms),
							 CStreamClientDiscovery(io_service),
							 m_session_name(session_name),
                             m_time_server_port(timeServerPort),
                             m_io_service(io_service)
{
}

CStreamServer::~CStreamServer()
{
}



void CStreamServer::onClientState(muroa::CStreamCtrlConnection* conn, const muroa::evClientState* evt) {
	bool has_changed = false;
	std::vector<CRenderClientDesc>::iterator it;
	string serviceName = conn->getServiceName();
	for(it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if(serviceName.compare( it->srvPtr->getServiceName() ) == 0 ) {
			it->isOnline(true);
            it->setVolume(evt->m_current_volume);
            it->setMemberOfSession(evt->m_member_of_session);
            has_changed = true;
		}
	}
	if(has_changed) {
		listClients();
	}
}


/**
 * throws CUnknownServeNameException if the IP address and port number for that servicename was not found
 */
void CStreamServer::connectToClient(std::string serviceName) {
	bip::tcp::endpoint endp = endpointOfService(serviceName);
	connectToClient(endp, serviceName);
}

/**
 * throws CUnknownServeNameException if the IP address and port number for that servicename was not found
 */
void CStreamServer::requestJoin(std::string serviceName) {
	try {
		CStreamCtrlConnection* conn = getCtrlConnByName(serviceName);
		requestJoin(conn);
	}
	catch(CUnknownServiceNameException ex) {
		LOG4CPLUS_ERROR( CApp::logger(), "request to join for unknown service name: '" << ex.reason() << "'");
	}
}

/**
 * throws CUnknownServeNameException if the IP address and port number for that servicename was not found
 */
void CStreamServer::requestLeave(std::string serviceName) {
	try {
		CStreamCtrlConnection* conn = getCtrlConnByName(serviceName);
		requestLeave(conn);
	}
	catch(CUnknownServiceNameException ex) {
		LOG4CPLUS_ERROR( CApp::logger(), "request to leave for unknown service name: '" << ex.reason() << "'");
	}
}


void CStreamServer::setVolume(std::string serviceName, int volume) {
    try {
        CStreamCtrlConnection* conn = getCtrlConnByName(serviceName );

        evSetVolume* evsv = new evSetVolume();
        evsv->m_volume = volume;

        conn->onEvent(evsv);
    }
    catch(CUnknownServiceNameException ex) {
        LOG4CPLUS_WARN(CApp::logger(), "CppServer::setVolume: no control connection to '" << serviceName << "' (" << ex.reason() << ")");
    }

}


void CStreamServer::connectToClient(bip::tcp::endpoint endp, const std::string& serviceName) {

	bool error = false;

	set<CStreamCtrlConnection*>::iterator iter2;
	for(iter2 = m_sessionless_connections.begin(); iter2 != m_sessionless_connections.end(); iter2++ ) {
		try {
			// there might be connections in the list that have already been closed or are not functional any more
			bip::tcp::endpoint known_endp = (*iter2)->remote_endpoint();
			if( known_endp == endp ) {
				// there is already a control connection open to this client, re request client state again ->
				// will get the same response (evCLientState) as if it would be a new connection
				evRequestClientState evcs;
				(*iter2)->sendEvent(&evcs);
				return;
			}
		}
		catch(...) {   //  boost::system::system_error syserr) {
			m_sessionless_connections.erase(iter2);
		}
	}

   	CStreamCtrlConnection* conn = new CStreamCtrlConnection(serviceName, this, m_io_service);
   	boost::system::error_code ec;
  	conn->connect(endp, ec);
   	if(ec) {
   		LOG4CPLUS_ERROR(CApp::logger(), "could not establish control connection to render client " << serviceName << " (" << endp << ")" );
   		delete conn;
   		throw CConnectionFailedException(serviceName, endp);
   	}
   	else {
   		conn->setup();
   		// conn->joinSession(0, "place session name here", bip::address() );
   		m_sessionless_connections.insert(conn);
   	}
}

void CStreamServer::requestJoin(CStreamCtrlConnection* conn) {
	evRequestJoin evj;
	evj.m_session_name = m_session_name;
	evj.m_timesrv_port = m_time_server_port;
	conn->onEvent(&evj);
}

void CStreamServer::requestLeave(CStreamCtrlConnection* conn) {
	evRequestLeave evl;
	evl.m_triggered_by_name = m_session_name;
	evl.m_session_name = m_session_name;
	conn->onEvent(&evl);
}

void CStreamServer::disconnectFromClient(const std::string& serviceName) {

    // first check, if it is a session member. If 'name' is member of our session,
    // let onClientLeftSession do the book keeping
    CStreamCtrlConnection* conn = CMediaStreamProvider::getCtrlConnection(serviceName);
    if(conn != 0) {
        evLeave ev;
        ev.m_triggered_by_session = "";
        ev.m_member_of_session = "";
        onClientLeftSession(conn, &ev);
    }

    try {
        // if 'name' was not part of the session, it might still be listed in m_sessionlesss_connections
        conn = getCtrlConnByName(serviceName);
        m_sessionless_connection_list_mutex.Lock();
        m_sessionless_connections.erase(conn);
        m_sessionless_connection_list_mutex.UnLock();

        delete conn;
    }
    catch(CUnknownServiceNameException ex) {
        LOG4CPLUS_ERROR(CApp::logger(), "could not disconnect control connection from render client '" << serviceName << "' (" << ex.reason() << ")" );
    }
}

/**
 * as soon as client appears on the network, try to establish a control connection
 */
void CStreamServer::onClientAppeared(ServDescPtr srvPtr) {

	LOG4CPLUS_INFO( CApp::logger(), "'" << srvPtr->getServiceName() << "' appeared on network" );

	bool already_in_m_rcs = false;
	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(srvPtr->getServiceName()) == 0 ) {
			if(*(it->srvPtr) == *srvPtr) {
				LOG4CPLUS_INFO( CApp::logger(), "    ...'" << srvPtr->getServiceName() << "' is known as session member -> request to join" );
			}
			else {
				LOG4CPLUS_WARN( CApp::logger(), "    ...'" << srvPtr->getServiceName() << "' is known as session member but with different address -> using new address and request to join" );
				it->srvPtr = srvPtr;
			}
			it->isOnline(false);  // it is online if we have a control connection and know its client state
			already_in_m_rcs = true;
		}
	}

	if(!already_in_m_rcs) {
		CRenderClientDesc rcd;
		rcd.isOnline(false);
		rcd.isMember(false);
		rcd.srvPtr = srvPtr;
		m_rcs.push_back(rcd);
	}

	bip::tcp::endpoint endp(srvPtr->getAddress(), srvPtr->getPortNr());
	connectToClient(endp, srvPtr->getServiceName());
}

/**
 * when a client disappears, clean up control connections if any are still open
 */
void CStreamServer::onClientDisappeared(ServDescPtr srvPtr) {

	LOG4CPLUS_INFO( CApp::logger(), "'" << srvPtr->getServiceName() << "' disappeared from the network" );
	disconnectFromClient(srvPtr->getServiceName());

	bool already_in_m_rcs = false;
	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(srvPtr->getServiceName()) == 0 ) {
			LOG4CPLUS_INFO( CApp::logger(), "   ...'" << srvPtr->getServiceName() << "' is member of session -> removing it" );

			it->isOnline(false);
			already_in_m_rcs = true;
		}
	}

	if(!already_in_m_rcs) {
		CRenderClientDesc rcd;
		rcd.isOnline(false);
		rcd.srvPtr = srvPtr;
		m_rcs.push_back(rcd);
	}
}


bool CStreamServer::endpointOfService(std::string serviceName, bip::tcp::endpoint& endp) {
	std::vector<CRenderClientDesc>::const_iterator it;
	for(it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if(it->isOnline()) {
			if(serviceName.compare( it->srvPtr->getServiceName() ) == 0 ) {
				endp = bip::tcp::endpoint( it->srvPtr->getAddress(), it->srvPtr->getPortNr() );
				if( endp.address().is_v4()) {
					/// @TODO: hack for now. Do not announce IPv6 address if not listening on it !!!
					return true;
				}
			}
		}
	}
	return false;
}

bip::tcp::endpoint CStreamServer::endpointOfService(const std::string& serviceName) {
	std::vector<CRenderClientDesc>::const_iterator it;
	for(it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if(it->isOnline()) {
			if(serviceName.compare( it->srvPtr->getServiceName() ) == 0 ) {
				bip::tcp::endpoint endp( it->srvPtr->getAddress(), it->srvPtr->getPortNr() );
				if( endp.address().is_v4()) {
					/// @TODO: hack for now. Do not announce IPv6 address if not listening on it !!!
					return endp;
				}
			}
		}
	}
	throw CUnknownServiceNameException(serviceName);
}


muroa::ServDescPtr CStreamServer::srvDescOfService(const std::string& serviceName) { // throws CUnknownServiceNameException
	std::vector<CRenderClientDesc>::const_iterator it;
	for(it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if(serviceName.compare( it->srvPtr->getServiceName() ) == 0 ) {
			return it->srvPtr;
		}
	}
	throw CUnknownServiceNameException(serviceName);
}

CRenderClientDesc CStreamServer::getRenderClientDescByName(const std::string& serviceName) { // throws CUnknownServiceNameException
	std::vector<CRenderClientDesc>::const_iterator it;
	for(it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if(serviceName.compare( it->srvPtr->getServiceName() ) == 0 ) {
			return *it;
		}
	}
	throw CUnknownServiceNameException(serviceName);
}

CStreamCtrlConnection* CStreamServer::getCtrlConnByName(const std::string& serviceName) {   // throws CUnknownServiceNameException
	CStreamCtrlConnection* sc = 0;

	set<CStreamCtrlConnection*>::iterator iter;
	for(iter = m_sessionless_connections.begin(); iter != m_sessionless_connections.end(); iter++ ) {
		sc = *iter;
		if( serviceName.compare( sc->getServiceName() ) == 0 ) {
			return sc;
		}
	}

	sc = CMediaStreamProvider::getCtrlConnection(serviceName);
	if(sc != 0) {
		return sc;
	}
	throw CUnknownServiceNameException(serviceName);
}

/** CStreamServer::addClient(CIPv4Address* addr)
 *
 * \return 0 if client was successfully added, 1 if client was already in the list and -1 if connection failed
 *
 **/
int CStreamServer::addClient(bip::tcp::endpoint endp, const std::string& name)
{
//    set<CStreamCtrlConnection*>::iterator iter;
//    for(iter = m_joined_connections.begin(); iter != m_joined_connections.end(); iter++ ) {
//      bip::tcp::endpoint known_endp = (*iter)->remote_endpoint();
//      if( known_endp == endp ) {
//          return 1;
//      }
//    }

	// checkEndp(endp);

    set<CStreamCtrlConnection*>::iterator iter2;
    for(iter2 = m_sessionless_connections.begin(); iter2 != m_sessionless_connections.end(); iter2++ ) {
      bip::tcp::endpoint known_endp = (*iter2)->remote_endpoint();
      if( known_endp == endp ) {
          return 1;
      }
    }

   	CStreamCtrlConnection* conn = new CStreamCtrlConnection(name, this, m_io_service);
   	boost::system::error_code ec;
  	conn->connect(endp, ec);
   	if(ec) {
   		LOG4CPLUS_ERROR(CApp::logger(), "could not establish control connection to render client " << name << " (" << endp << ")" );
   		delete conn;
   		return -1;
   	}
   	else {
   		conn->setup();
   		// conn->joinSession(0, "place session name here", bip::address() );
   		m_sessionless_connections.insert(conn);
   		return 0;
   	}
}


///** remove client irrespective of it is a session member or not
// *  called when tcp ctrl connection was closed by the other end.
// */
//void CStreamServer::removeClient(muroa::CStreamCtrlConnection* connPtr) {
//	// first check, if it is a session member
//	int num_removed = removeJoinedConnection(connPtr);
//
//    m_sessionless_connection_list_mutex.Lock();
//    m_sessionless_connections.erase(connPtr);
//    m_sessionless_connection_list_mutex.UnLock();
//
//    delete connPtr;
//}
//
//
//void CStreamServer::removeClientByName(const string &name)
//{
//    // try to find the control connection in the joined connections. If 'name' is member of our session,
//    // let onClientLeftSession do the book keeping
//	CStreamCtrlConnection* conn = CMediaStreamProvider::getCtrlConnection(name);
//	if(conn != 0) {
//        evLeave ev;
//        ev.m_triggered_by_session = "";
//        ev.m_member_of_session = "";
//        onClientLeftSession(conn, &ev);
//    }
//
//    try {
//        // if 'name' was not part of the session, it might still be listed in m_sessionlesss_connections
//        conn = getCtrlConnByName(name);
//        m_sessionless_connection_list_mutex.Lock();
//        m_sessionless_connections.erase(conn);
//        m_sessionless_connection_list_mutex.UnLock();
//
//        delete conn;
//    }
//    catch(CUnknownServiceNameException ex) {
//        LOG4CPLUS_ERROR(CApp::logger(), "removeCLientByName: control connection not available '" << ex.reason( )<< "'");
//    }
//}

void CStreamServer::onClientRejectedSessionMember( muroa::CStreamCtrlConnection* conn, const muroa::evJoinRejected* evt) {
	LOG4CPLUS_INFO( CApp::logger(), "'" << conn->getServiceName() << "' rejected to become session member" );
	bool has_changed = false;

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->isMember(false);
			has_changed = true;
		}
	}

	if(has_changed) {
		listClients();
	}

}

void CStreamServer::onClientBecameSessionMember( muroa::CStreamCtrlConnection* conn, const muroa::evSessionState* evt) {

	LOG4CPLUS_INFO( CApp::logger(), "'" << conn->getServiceName() << "' became session member" );
	bool has_changed = false;
    // remove conn from the list of connections not associated with a session
    m_sessionless_connection_list_mutex.Lock();
	int num_ptr = m_sessionless_connections.erase(conn);
    m_sessionless_connection_list_mutex.UnLock();
	if(num_ptr > 0) {
		// it was found in the list of connections not associated with a session
		addJoinedConnection(conn);
	}

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->isMember(true);
            it->setMemberOfSession(m_session_name);
			has_changed = true;
		}
	}

	if(has_changed) {
		listClients();
	}
}

void CStreamServer::onClientLeftSession(muroa::CStreamCtrlConnection* conn, const muroa::evLeave* evt) {
	LOG4CPLUS_INFO( CApp::logger(), "'" << conn->getServiceName() << "' left session" );
	bool has_changed = false;

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->isMember(false);
            it->setMemberOfSession("");
			has_changed = true;
		}
	}

	removeJoinedConnection(conn);
    m_sessionless_connection_list_mutex.Lock();
	m_sessionless_connections.insert(conn);
    m_sessionless_connection_list_mutex.UnLock();

	if(has_changed) {
		listClients();
	}
}

void CStreamServer::onTimeout(muroa::CStreamCtrlConnection* conn, const evTimeout* evt) {

}

void  CStreamServer::onError(muroa::CStreamCtrlConnection* conn, const evJoinRejected* evt) {

}

void  CStreamServer::onError(muroa::CStreamCtrlConnection* conn, const evError* evt) {

}

void CStreamServer::onSessionError(muroa::CStreamCtrlConnection* conn, const evSessionError* evt) {
	bool has_changed = false;

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			if(it->getLastErrorMsg().compare(evt->m_error_msg) != 0) {
				it->setLastErrorMsg(evt->m_error_msg);
				has_changed = true;
			}
		}
	}

	if(has_changed) {
		listClients();
	}
}

void  CStreamServer::onVolume(muroa::CStreamCtrlConnection* conn, const evVolume* evt) {
	bool rcd_found = false;
	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->setVolume( evt->m_volume );
			rcd_found = true;
		}
	}

	if(rcd_found) {
		listClients();
	}
}


//
//std::vector<CRenderClientDesc> CStreamServer::getRenderClients() {
//
//	vector<string> service_names_joined = getJoinedRenderClients();
//	vector<CRenderClientDesc> rcs;
//
//	for(vector<string>::const_iterator it = service_names_joined.begin(); it != service_names_joined.end(); it++) {
//		try {
//			CRenderClientDesc rcd( getRenderClientDescByName(*it) );
//			rcs.push_back(rcd);
//		}
//		catch(CUnknownServiceNameException ex) {
//			LOG4CPLUS_ERROR(CApp::logger(), "error: service description not available '" << ex.reason( )<< "'");
//		}
//	}
//
//	set<CStreamCtrlConnection*>::iterator iter;
//	m_sessionless_connection_list_mutex.Lock();
//	for(iter = m_sessionless_connections.begin(); iter != m_sessionless_connections.end(); iter++ ) {
//		try {
//			CRenderClientDesc rcd = getRenderClientDescByName((*iter)->getServiceName());
//			rcs.push_back(rcd);
//		}
//		catch(CUnknownServiceNameException ex) {
//			LOG4CPLUS_ERROR(CApp::logger(), "error: service description not available '" << ex.reason( )<< "'");
//		}
//	}
//	m_sessionless_connection_list_mutex.UnLock();
//
//	return rcs;
//}


}// namespace muroa

