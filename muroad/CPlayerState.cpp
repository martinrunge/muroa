/*
 Copyright (c) 2002-2014 "Martin Runge"

 CPlayerState.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include <CPlayerState.h>

#include "CApp.h"
#include "CSettings.h"
#include "cplayer.h"
#include "CTcpServer.h"
#include "CCtrlConnection.h"
#include "avahi/CDnsSdAvahi.h"

#include <string>

using namespace std;
using namespace muroa;

namespace muroa {

CPlayerState::CPlayerState(boost::asio::io_service& io_service) :           m_tcp_server(0),
		                                                                    m_io_service(io_service),
																			m_player(0) {

	boost::asio::ip::tcp protocol = tcp::v4();
	if( CApp::settings().getConfigVal("muroad.useIPv6", "no").compare(string("yes")) == 0) {
		protocol = tcp::v6();
	}
	tcp::endpoint endp = tcp::endpoint(protocol, CApp::settings().getConfigVal("muroad.ControlPort", 5555));
	m_tcp_server = new CTcpServer(io_service, &m_conn_mgr, endp, reinterpret_cast<factory_ptr_t>(&CCtrlConnection::create));

    // CTcpServer searches for a free port to bind to starting at the given port.
	// endp is passed as reference and after return contains the port that was actually was bound.
	CApp::settings().setPersistentVal(string("muroad.ControlPort"), (const int)endp.port());

	string serviceName = CApp::settings().getConfigVal("muroad.ServiceName", "Muroa Streaming Client");
	string serviceType = CApp::settings().getConfigVal("muroad.ServiceType", "_muroad._tcp");
//	string serviceType = CApp::settings().getConfigVal("muroad.ServiceType", "_muroad._udp");

     m_dnssd = new CDnsSdAvahi(io_service, serviceName, endp.port(), serviceType, vector<string>());
//    m_dnssd = new CDnsSdAvahi(io_service, serviceName, 44400, serviceType, vector<string>());
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CApp::serviceChanged, CApp::getInstPtr()));

	startPlayer();
}

CPlayerState::~CPlayerState() {

	delete m_dnssd;

	delete m_tcp_server;
	if(m_player != 0) {
		delete m_player;
		m_player = 0;
	}
}


void CPlayerState::startPlayer() {
	assert(m_player == 0);
	m_player = new CPlayer(m_io_service);
}

void CPlayerState::stopPlayer() {
	if(m_player != 0) {
		delete m_player;
		m_player = 0;
	}
}

} /* namespace muroa */
