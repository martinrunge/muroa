/*
 Copyright (c) 2002-2014 "Martin Runge"

 CTimeServiceCtrl.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CTimeServiceCtrl.h"
#include "CTimeService.h"

using namespace std;
using namespace log4cplus;

namespace muroa {

CTimeServiceCtrl::CTimeServiceCtrl() : m_server_role(false), m_used_port(0) {
	m_logger =  Logger::getInstance("main");
}

CTimeServiceCtrl::~CTimeServiceCtrl() {

}

void CTimeServiceCtrl::start(bool server_role, boost::asio::ip::address ip_address, int portNr, boost::asio::ip::udp protocol) {
	m_server_role = server_role;
	m_thread = new std::thread(bind(&CTimeServiceCtrl::threadfunc, this, server_role, ip_address, portNr, protocol));
}

void CTimeServiceCtrl::stop() {
	//m_thread->
	delete m_thread;
}

void CTimeServiceCtrl::threadfunc(bool server_role, boost::asio::ip::address ip_address, int portNr, boost::asio::ip::udp protocol) {
	try {
		boost::asio::io_service io_service;
		CTimeService ts(io_service, server_role, ip_address, portNr, protocol);
		LOG4CPLUS_DEBUG(m_logger, "starting TimeService");
		if(server_role) {
			m_used_port = ts.getUsedPort();
			m_cond_var.notify_all();
		}
		io_service.run();
		LOG4CPLUS_INFO(m_logger, "shutting down time service.");
	} catch (std::exception& e) {
		LOG4CPLUS_ERROR(m_logger, "Uncaught exception from TimeServices mainloop: " << e.what());
	}
}

int CTimeServiceCtrl::getCurrentServerPort() {
	if(m_server_role == true) {
		if( m_used_port == 0) {
			// port has not yet been set by time service thread.
			unique_lock<mutex> _lock(m_mutex);
			while( m_used_port == 0) {
				m_cond_var.wait(_lock);
			}
		}
	}
	return m_used_port;
}


} /* namespace muroa */
