/*
Copyright (c) 2002-2014 "Martin Runge"

server.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

Muroa is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "CppServer.h"

#include <log4cplus/loggingmacros.h>
#include <jsoncpp/json/json.h>

#include <CApp.h>
#include "CWSMsgHandler.h"

using namespace muroa;
using namespace std;



CppServer::CppServer(boost::asio::io_service& io_service, vector<string> clients, int timeServerPort, int sessionID )
                  : CStreamServer(io_service, timeServerPort, sessionID, 2000),
                    m_io_service(io_service),
					m_decoder(0),
					m_selected_clients(clients)
{
	m_selected_clients = CApp::settings().getPersisentVal("msessiond", m_selected_clients);
}

CppServer::~CppServer() {
	stopStream();
}

// void CppServer::setWSSrv(muroa::WSSrv *wssrv) {
// 	m_ws_msg_handler = wssrv;
// }

void CppServer::addClientByName(std::string serviceName) {
	bip::tcp::endpoint endp;
	if( endpointOfService(serviceName, endp) ) {
		// got endpoint for service name
		addClient(endp, serviceName);
		addClientToSelected(serviceName);
	}
	else {
		LOG4CPLUS_WARN(CApp::logger(), "CppServer::addClientByName: could not determine TCP endpoint of service: '" << serviceName << "'");
	}

}

void CppServer::removeClient(std::string serviceName) {

	CStreamServer::removeClient(serviceName);
	LOG4CPLUS_INFO(CApp::logger(), "CppServer::removeClient('" << serviceName << "')");
	removeClientFromSelected(serviceName);
}


void CppServer::playStream(std::string url) {
	muroa::CStreamFmt new_sfmt;

	LOG4CPLUS_INFO(CApp::logger(), "Play stream: " << url);


	if(m_decoder != 0) {
		flush();
		delete m_decoder;
		close();
	}
	m_decoder = new CStreamDecoder(this);

	LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: about to open url: " << url);
	new_sfmt = m_decoder->open(url);

	LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: url opened: channels: " << new_sfmt.numChannels << " samplerate: " << new_sfmt.sampleRate);

	if(new_sfmt.isValid()) {
		if(isOpen() == false) {
			open(new_sfmt);
		}

		muroa::CStreamFmt old_sfmt(getStreamFmt());
		if( new_sfmt != old_sfmt) {
			close();
			open(new_sfmt);
		}

		m_decoder->startDecodingThread();
	}
}

void CppServer::stopStream() {
	LOG4CPLUS_INFO( CApp::logger(), "Stop stream" );
	flush();
	if(m_decoder != 0) {
		delete m_decoder;
		m_decoder = 0;
	}
	close();
}

void CppServer::clientRejectedSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evJoinRejected* evt) {
	CStreamServer::clientRejectedSessionMember(conn, evt);

	LOG4CPLUS_INFO( CApp::logger(), "'" << conn->getServiceName() << "' rejected to become session member" );

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->isMember(false);
		}
	}

	m_ws_msg_handler->listClients();
}

void CppServer::clientBecameSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evSessionState* evt) {
	CStreamServer::clientBecameSessionMember(conn, evt);

	LOG4CPLUS_INFO( CApp::logger(), "'" << conn->getServiceName() << "' became session member" );

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->isMember(true);
		}
	}

	m_ws_msg_handler->listClients();
}

void CppServer::clientLeftSession(muroa::CStreamCtrlConnection* conn, const muroa::evLeave* evt) {
	CStreamServer::clientLeftSession(conn, evt);

	LOG4CPLUS_INFO( CApp::logger(), "'" << conn->getServiceName() << "' left session" );

	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(conn->getServiceName()) == 0 ) {
			it->isMember(false);
		}
	}

	m_ws_msg_handler->listClients();
}

void CppServer::reportError(muroa::CStreamCtrlConnection* conn, const evJoinRejected* evt) {

}

void CppServer::reportClientState(muroa::CStreamCtrlConnection* conn, const muroa::CmdStreamBase* evt) {
	if(typeid(*evt) == typeid(evClientState)) {
		const evClientState* ct = reinterpret_cast<const evClientState*>(evt);

		evRequestJoin* evj = new evRequestJoin();
		evj->m_session_name = "testsession";
		// evj->m_mcast_addr =
		evj->m_timesrv_port = 12345;
		conn->onEvent(evj);
	}
}

void CppServer::reportProgress( int posInSecs, int durationInSecs) {
	return ;
}


void CppServer::onClientAppeared(ServDescPtr srvPtr) {

	LOG4CPLUS_INFO( CApp::logger(), "'" << srvPtr->getServiceName() << "' appeared on network" );

	bool already_in_m_rcs = false;
	for(vector<CRenderClientDesc>::iterator it = m_rcs.begin(); it != m_rcs.end(); it++) {
		if( it->srvPtr->getServiceName().compare(srvPtr->getServiceName()) == 0 ) {
			LOG4CPLUS_INFO( CApp::logger(), "    ...'" << srvPtr->getServiceName() << "' is known as session member -> request to join" );
			it->isOnline(true);
			already_in_m_rcs = true;
		}
	}

	if(!already_in_m_rcs) {
		CRenderClientDesc rcd;
		rcd.isOnline(true);
		rcd.isMember(false);
		rcd.srvPtr = srvPtr;
		m_rcs.push_back(rcd);
	}


	for(vector<string>::iterator it = m_selected_clients.begin(); it != m_selected_clients.end(); it++) {
		string appeared_service_name = srvPtr->getServiceName();
		if( (*it).compare(appeared_service_name) == 0 )
		{
			m_clients.push_back(srvPtr);

			bip::tcp::endpoint endp(srvPtr->getAddress(), srvPtr->getPortNr() );
			addClient(endp, srvPtr->getServiceName());
			storeClientList();
		}
	}
	m_ws_msg_handler->listClients();
}

void CppServer::onClientDisappeared(ServDescPtr srvPtr) {

	LOG4CPLUS_INFO( CApp::logger(), "'" << srvPtr->getServiceName() << "' disappeared from the network" );

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
	m_ws_msg_handler->listClients();
}

void CppServer::onClientChanged() {

}

bool CppServer::endpointOfService(std::string serviceName, bip::tcp::endpoint& endp) {
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

void CppServer::addClientToSelected(const string& serviceName) {

	vector<string>::iterator it = std::find(m_selected_clients.begin(), m_selected_clients.end(), serviceName);
	if ( it == m_selected_clients.end() ) {
		// serviceName is not yet m_selected_clients
		m_selected_clients.push_back(serviceName);
	}
	storeClientList();
}

void CppServer::removeClientFromSelected(const string& serviceName) {

	vector<string>::iterator it;
	do {
		it = std::find(m_selected_clients.begin(), m_selected_clients.end(), serviceName);
		if ( it != m_selected_clients.end() ) {
			// serviceName is not yet m_selected_clients
			m_selected_clients.erase(it);
			storeClientList();
		}
	} while(it != m_selected_clients.end());
}


void CppServer::storeClientList() {
	CApp::settings().setPersistentVal("msessiond", m_selected_clients);
}

