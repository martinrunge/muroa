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
#include "CPlayerState.h"
#include <cmds/StreamCtrlIDs.h>

using namespace muroa;
using namespace std;

CCtrlConnection::CCtrlConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service) {

}

CCtrlConnection::~CCtrlConnection() {
	// shutdown();
}


void CCtrlConnection::onSetup(uint32_t cmdID) {
	ack(cmdID);
}

void CCtrlConnection::onShutdown(uint32_t cmdID) {
	// m_player_state->shutDown();
	ack(cmdID);
}

void CCtrlConnection::onAck(uint32_t cmdID) {
}

void CCtrlConnection::onError(uint32_t cmdID, int errorCode, std::string errmsg) {
}

void CCtrlConnection::onJoinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) {
	int rc = m_player_state->requestJoinSession(name, this);
	if(rc == 0 || rc == 1) {
		ack(cmdID);
		setRTPPort(StreamCtrlIDs::getCmdID(), m_player_state->getRTPPort());
	}
	else {
		if(rc == -1) {
			ostringstream oss;
			oss << "already member of session '" << m_player_state->getSessionName() << "'";
			error(cmdID, StreamCtrlIDs::EC_MEMBER_OF_OTHER_SESSION, oss.str() );
		}
		else {
			error(cmdID, StreamCtrlIDs::EC_OUT_OF_SERVICE, "out of service" );
		}
	}
}

void CCtrlConnection::onJoinSessionLeave() {
	int rc = m_player_state->requestLeaveSession(this);
}

void CCtrlConnection::onTakeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) {
}

void CCtrlConnection::onSetTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port) {
}

void CCtrlConnection::onGetTimeSrv(uint32_t cmdID) {
}

void CCtrlConnection::onGetRTPPort(uint32_t cmdID) {
}

void CCtrlConnection::onSetRTPPort(uint32_t cmdID, uint32_t port) {
}

void CCtrlConnection::onJoinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
}

void CCtrlConnection::onLeaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
}

void CCtrlConnection::onGetMCastGrp(uint32_t cmdID) {
}

void CCtrlConnection::onSetStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts) {
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
