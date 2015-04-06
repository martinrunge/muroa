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

CStreamCtrlConnection::CStreamCtrlConnection(std::string serviceName, CStreamServer* stream_server, boost::asio::io_service& io_service)
                      : bip::tcp::socket(io_service),
						m_stream_connection(0),
					    m_stream_server(stream_server),
						m_serviceName(serviceName),
						m_RTP_port(0)
{

}

CStreamCtrlConnection::~CStreamCtrlConnection() {

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


void muroa::CStreamCtrlConnection::onOpen(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onClose(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onAck(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onError(uint32_t cmdID, int errorCode, std::string errmsg) {
}

void muroa::CStreamCtrlConnection::onJoinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) {
}

void muroa::CStreamCtrlConnection::onJoinSessionLeave() {
}

void muroa::CStreamCtrlConnection::onTakeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) {
}

void muroa::CStreamCtrlConnection::onSetTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port) {
}

void muroa::CStreamCtrlConnection::onGetTimeSrv(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onGetRTPPort(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onSetRTPPort(uint32_t cmdID, uint32_t port) {
}

void muroa::CStreamCtrlConnection::onJoinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
}

void muroa::CStreamCtrlConnection::onLeaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
}

void muroa::CStreamCtrlConnection::onGetMCastGrp(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onSetStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts) {
}

void muroa::CStreamCtrlConnection::onGetStreamTimeBase(uint32_t cmdID,
		uint32_t ssrc) {
}

void muroa::CStreamCtrlConnection::onResetStream(uint32_t cmdID,
		uint32_t ssrc) {
}

void muroa::CStreamCtrlConnection::onGetVolume(uint32_t cmdID) {
}

void muroa::CStreamCtrlConnection::onSetVolume(uint32_t cmdID, int percent) {
}


void CStreamCtrlConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	newData(buffer.data(), length);
}

void CStreamCtrlConnection::onDataToSend(const char* data, int len) {
	boost::asio::write(*this, boost::asio::buffer(data, len));
}

} /* namespace muroa */

