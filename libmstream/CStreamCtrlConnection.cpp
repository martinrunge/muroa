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
    // m_srv_sm.stop();
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
	assert(m_stream_connection == 0);
	m_stream_connection = new CStreamConnection(this, m_RTP_port);

	boost::system::error_code ec;
	bip::tcp::endpoint endpoint = remote_endpoint(ec);
	if (ec)	{
	  // An error occurred.
	}
	else {
		bip::address addr = endpoint.address();
		if(addr.is_v4()) {
			CIPv4Address v4addr(addr.to_v4(), m_RTP_port);
			m_stream_connection->connect(&v4addr);
		}
	}
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

void CStreamCtrlConnection::ontError(const evError* evt) {
	m_stream_server->onError(this, evt);
}

void CStreamCtrlConnection::onError(const evJoinRejected* evt) {
	m_stream_server->onError(this, evt);
}

void CStreamCtrlConnection::onTimeout(const evTimeout* evt) {

}

void CStreamCtrlConnection::onClientState(const evClientState* evt) {
	m_stream_server->onClientState(this, evt);
}

void CStreamCtrlConnection::onClientState(const evLeave* evt) {
	// closeStreamConnection();
	m_stream_server->onClientLeftSession(this, evt);
}

void CStreamCtrlConnection::sendJoinRequest(const evRequestJoin* evt) {
    sendEvent(evt);
}

void CStreamCtrlConnection::setVolume(const evSetVolume* evt) {
	sendEvent(evt);
}

void CStreamCtrlConnection::onVolume(const evVolume* evt) {
    m_stream_server->onVolume(this, evt);
}

void CStreamCtrlConnection::sendLeaveRequest(const muroa::evRequestLeave* evt) {
	sendEvent(evt);
}


void CStreamCtrlConnection::onBecameSessionMember(const CmdStreamBase *cmd) {

	if(typeid(*cmd) == typeid(evSessionState)) {
		const evSessionState* evt = reinterpret_cast<const evSessionState*>(cmd);
		if(evt->m_rtp_unicast_port != 0) {
			// use unicast
			m_use_multicast_rtp = false;
			m_RTP_port = evt->m_rtp_unicast_port;
			if(m_RTP_port != 0) {
				openStreamConnection();
			}
		}
		else {
			// this lcient is served via multicast RTP
			m_use_multicast_rtp = true;
		}
		m_stream_server->onClientBecameSessionMember(this, evt);
	}
}


void CStreamCtrlConnection::onLeftSession(const muroa::CmdStreamBase* cmd) {
    if(typeid(*cmd) == typeid(evLeave)) {
        const evLeave* evt = reinterpret_cast<const evLeave*>(cmd);
        closeStreamConnection();
        m_stream_server->onClientLeftSession(this, evt);
    }
}


void CStreamCtrlConnection::onJoinRejected(const muroa::CmdStreamBase *cmd) {
    if(typeid(*cmd) == typeid(evJoinRejected)) {
        const evJoinRejected* evt = reinterpret_cast<const evJoinRejected*>(cmd);
        m_stream_server->onClientRejectedSessionMember(this, evt);
    }
}

	void CStreamCtrlConnection::sendAck(const evJoinAccepted* evt) {
	evAck* ack = new evAck();
	ack->m_cmd_id = evt->getID();

	sendEvent(ack);
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

            // ask io_service to delete this instance of CStreamCtrlConnection later. It might have bee deleted by then triggered
            // by onClientDisappeared called by avahi. To avoid double deletion pass the connection name instead of the pointer
            // and search for a ctrl connection with that name later. If it has already been deleted, no pointer will be found.
            get_io_service().post(boost::bind(&CStreamServer::disconnectFromClient, m_stream_server, m_serviceName ));
    	}
    	// LOG4CPLUS_ERROR(CApp::logger(), "error in handle_read:  " << error.message());
    	// delete this;
    }
}



} /* namespace muroa */

