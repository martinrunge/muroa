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

using namespace muroa;
using namespace std;



CppServer::CppServer(boost::asio::io_service& io_service, vector<bip::tcp::endpoint> clients, int timeServerPort, int sessionID )
                  : CStreamServer(io_service, timeServerPort, sessionID),
                    m_io_service(io_service),
					m_decoder(0)
{

	for(int i = 0; i < clients.size(); i++) {
		addClient(clients[i], "example session");
	}
}

CppServer::~CppServer() {
	stopStream();
}

void CppServer::playStream(std::string url) {
	muroa::CStreamFmt new_sfmt;

	if(m_decoder != 0) {
		delete m_decoder;
	}
	m_decoder = new CStreamDecoder(this);
	m_decoder->open(url);

	if(m_decoder->isOpen()) {
		new_sfmt = m_decoder->getStreamFmt();
	}

	if(isOpen() == false) {
		open(new_sfmt);
	}

	muroa::CStreamFmt old_sfmt(getStreamFmt());
	if( new_sfmt != old_sfmt) {
		close();
		open(new_sfmt);
	}

}

void CppServer::stopStream() {
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


