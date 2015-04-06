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

#include <CCtrlConnection.h>

CCtrlConnection::CCtrlConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service) {

}

CCtrlConnection::~CCtrlConnection() {

}


void CCtrlConnection::onOpen(uint32_t cmdID) {
}

void CCtrlConnection::onClose(uint32_t cmdID) {
}

void CCtrlConnection::onAck(uint32_t cmdID) {
}

void CCtrlConnection::onError(uint32_t cmdID, int errorCode,
		std::string errmsg) {
}

void CCtrlConnection::onJoinSession(uint32_t cmdID, std::string name,
		boost::asio::ip::address session_srv) {
}

void CCtrlConnection::onJoinSessionLeave() {
}

void CCtrlConnection::onTakeFromSession(uint32_t cmdID, std::string name,
		boost::asio::ip::address session_srv) {
}

void CCtrlConnection::onSetTimeSrv(uint32_t cmdID,
		boost::asio::ip::address session_srv, uint32_t port) {
}

void CCtrlConnection::onGetTimeSrv(uint32_t cmdID) {
}

void CCtrlConnection::onGetRTPPort(uint32_t cmdID) {
}

void CCtrlConnection::onSetRTPPort(uint32_t cmdID, uint32_t port) {
}

void CCtrlConnection::onJoinMCastGrp(uint32_t cmdID,
		boost::asio::ip::address mcast_addr) {
}

void CCtrlConnection::onLeaveMCastGrp(uint32_t cmdID,
		boost::asio::ip::address mcast_addr) {
}

void CCtrlConnection::onGetMCastGrp(uint32_t cmdID) {
}

void CCtrlConnection::onSetStreamTimeBase(uint32_t cmdID, uint32_t ssrc,
		uint64_t rtp_ts, uint64_t pts) {
}

void CCtrlConnection::onGetStreamTimeBase(uint32_t cmdID, uint32_t ssrc) {
}

void CCtrlConnection::onResetStream(uint32_t cmdID, uint32_t ssrc) {
}

void CCtrlConnection::onGetVolume(uint32_t cmdID) {
}

void CCtrlConnection::onSetVolume(uint32_t cmdID, int percent) {
}




void CCtrlConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	newData(buffer.data(), length);
}

void CCtrlConnection::onDataToSend(const char* data, int len) {
	writeData(data, len);
}
