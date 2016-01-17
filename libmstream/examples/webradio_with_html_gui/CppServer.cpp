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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "CppServer.h"

#include <CApp.h>

using namespace muroa;
using namespace std;



CppServer::CppServer(boost::asio::io_service& io_service, vector<string> clients, int timeServerPort, int sessionID )
                  : CStreamServer(io_service, timeServerPort, sessionID, 2000),
                    m_io_service(io_service),
					m_decoder(0),
					m_selected_clients(clients)
{
}

CppServer::~CppServer() {
	stopStream();
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
	flush();
	if(m_decoder != 0) {
		delete m_decoder;
		m_decoder = 0;
	}
	close();
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
	for(vector<string>::iterator it = m_selected_clients.begin(); it != m_selected_clients.end(); it++) {

		string appeared_service_name = srvPtr->getServiceName();
		if( (*it).compare(appeared_service_name) == 0 )
		{
			m_clients.push_back(srvPtr);

			bip::tcp::endpoint endp(srvPtr->getAddress(), srvPtr->getPortNr() );
			addClient(endp, srvPtr->getServiceName());
		}
	}
}

void CppServer::onClientDisappeared(ServDescPtr srvPtr) {

}

void CppServer::onClientChanged() {

}

