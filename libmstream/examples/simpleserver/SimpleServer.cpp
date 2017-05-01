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

#include "SimpleServer.h"

using namespace muroa;
using namespace std;



SimpleServer::SimpleServer(vector<bip::tcp::endpoint> clients, string session_name, int timeServerPort, int sessionID ) : CStreamServer(m_io_service, session_name, timeServerPort, sessionID) {
	try {

		for(int i = 0; i < clients.size(); i++) {
			addClient(clients[i], "example session");
		}

		m_in_fd = fopen("infile.raw", "r");
		if(m_in_fd == NULL) {
			perror("fopen('infile.raw')");
		}
		open(2, 48000, AV_SAMPLE_FMT_S16);

		boost::asio::deadline_timer t(m_io_service, boost::posix_time::milliseconds(10) );
		t.async_wait( boost::bind(&SimpleServer::sendData, this) );
	} catch (std::exception& e) {
		cerr << "Uncaught exception from mainloop: " << e.what() << endl;
	}
}
SimpleServer::~SimpleServer() {
	fclose(m_in_fd);
    close();

}

void SimpleServer::run() {
	m_io_service.run();
}

void SimpleServer::sendData()
{
	  unsigned long num;
	  int buffersize = 1024;
	  char buffer[buffersize];

	  num = fread((void*)buffer, buffersize, 1, m_in_fd);
	  write(buffer, num * buffersize);

	  if(num > 0) {
			boost::asio::deadline_timer t(m_io_service, boost::posix_time::milliseconds(10) );
			t.async_wait( boost::bind(&SimpleServer::sendData, this) );
	  }
}

void SimpleServer::onClientState(muroa::CStreamCtrlConnection* conn, const muroa::CmdStreamBase* evt) {
	if(typeid(*evt) == typeid(evClientState)) {
		const evClientState* ct = reinterpret_cast<const evClientState*>(evt);

		evRequestJoin* evj = new evRequestJoin();
		evj->m_session_name = "testsession";
		// evj->m_mcast_addr =
		evj->m_timesrv_port = 12345;
		conn->onEvent(evj);
	}

}

