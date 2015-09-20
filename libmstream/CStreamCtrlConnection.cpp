/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCtrlConnection.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CStreamCtrlConnection.h"
#include "CStreamConnection.h"
#include "CStreamServer.h"

namespace muroa {

CStreamCtrlConnection::CStreamCtrlConnection(std::string serviceName, muroa::CStreamServer* stream_server, boost::asio::io_service& io_service)
                      : bip::tcp::socket(io_service),
                        m_stream_connection(0),
					    m_stream_server(stream_server),
						m_serviceName(serviceName),
						m_srv_sm(this),
						m_RTP_port(0)
{

}

CStreamCtrlConnection::~CStreamCtrlConnection() {
	closeStreamConnection();
	// close(); ?
}

void CStreamCtrlConnection::connect(const endpoint_type & peer_endpoint, boost::system::error_code & ec) {
	bip::tcp::socket::connect(peer_endpoint, ec);
	if(!ec) {
		m_srv_sm.start();
		m_srv_sm.pstate();
		// m_srv_sm.process_event( evClientState() );
		start_read();
	}
}




void CStreamCtrlConnection::openStreamConnection() {

// Hier weitermachen: RTP stream connection gemäß der Daten über Ctrl Connection aufbauen.

	assert(m_stream_connection == 0);
	m_stream_connection = new CStreamConnection(this, m_RTP_port);
}

void CStreamCtrlConnection::closeStreamConnection() {
	if(m_stream_connection != 0) {
		delete m_stream_connection;
		m_stream_connection = 0;
	}
}


void CStreamCtrlConnection::onSetup() {
}

void CStreamCtrlConnection::onShutdown() {
}

bool CStreamCtrlConnection::onEvent(muroa::CmdStreamBase* ev) {
	m_srv_sm.onEvent(ev);
	return true;
}


void CStreamCtrlConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	newData(buffer.data(), length);
}

void CStreamCtrlConnection::onDataToSend(const char* data, int len) {
	boost::asio::write(*this, boost::asio::buffer(data, len));
}

void CStreamCtrlConnection::reportError(std::string) {

}

void CStreamCtrlConnection::reportTimeout(std::string) {

}

void CStreamCtrlConnection::reportClientState(const CmdStreamBase* evt) {
	m_stream_server->reportClientState(this, evt);
}

void CStreamCtrlConnection::requestJoin(const evRequestJoin* evt) {
	sendEvent(evt);
}


void CStreamCtrlConnection::start_read() {
	async_read_some(boost::asio::buffer(m_buffer),
                    boost::bind(&CStreamCtrlConnection::handle_read, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
}


void CStreamCtrlConnection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
    	dataReceived(m_buffer, bytes_transferred);
    	start_read();
    }
    else {
    	if(error == boost::asio::error::eof ) {
    		// connection was closed
    		assert(m_stream_server != 0);
    		m_stream_server->removeClient(this);
    	}
    	// LOG4CPLUS_ERROR(CApp::logger(), "error in handle_read:  " << error.message());
    	// delete this;
    }
}



} /* namespace muroa */

