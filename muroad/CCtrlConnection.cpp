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
#include "CPlayer.h"
#include <cmds/StreamCtrlIDs.h>

using namespace muroa;
using namespace std;

CCtrlConnection::CCtrlConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service), m_clnt_sm(this) {

}

CCtrlConnection::~CCtrlConnection() {
}

void CCtrlConnection::start() {
	CTcpConnection::start();
	setup();
	m_clnt_sm.start();
}


void CCtrlConnection::shutdown() {
    // will call "on_exit" of currently active states
    m_clnt_sm.stop();
	shutdown();
	CTcpConnection::stop();
}


void CCtrlConnection::onClose() {
	m_player->onCloseCtrlConn(this);
}

void CCtrlConnection::onSetup() {
	// ack(cmdID);
}

void CCtrlConnection::onShutdown() {
	// m_player_state->shutDown();
	// ack(cmdID);
}

bool CCtrlConnection::onEvent(CmdStreamBase* ev) {
	m_clnt_sm.onEvent(ev);
	return true;
}

void CCtrlConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	newData(buffer.data(), length);
}

void CCtrlConnection::onDataToSend(const char* data, int len) {
	writeData(data, len);
}

// IClientSMActions
void CCtrlConnection::sendClientState() {
	evClientState *ev = new evClientState();

	ev->m_member_of_session = m_player->getSessionName();
	ev->m_current_volume    = m_player->getVolume();
	ev->m_session_srv       = m_player->getSessionServer();

	sendEvent(ev);
}

void CCtrlConnection::confirmJoinRequest(const evRequestJoin& evt) {
	m_player->requestJoinSession(evt.m_session_name, this);
	evJoinAccepted *ev = new evJoinAccepted();
	sendEvent(ev);
}

bool CCtrlConnection::mayJoinSession(const evRequestJoin& rj) {
	return m_player->mayJoinSession(rj, this);
}


void CCtrlConnection::becomeSessionMember(const evRequestJoin& evt) {
	m_player->becomeSessionMember(evt, this);

	evSessionState* sstate = new evSessionState();
	sstate->m_session_name = m_player->getSessionName();
	sstate->m_timesrv_port = evt.m_timesrv_port;
	sstate->m_volume = m_player->getVolume();
	if(m_player->multicastSupported()) {
		sstate->m_rtp_unicast_port = 0;
	}
	else {
		sstate->m_rtp_unicast_port = m_player->getRTPUnicastPort();
	}

	sendEvent(sstate);
}


void CCtrlConnection::sendEvSessionError(const muroa::evSessionError& evt) {
    sendEvent(&evt);
}


void CCtrlConnection::leaveSession(const evRequestLeave& evt) {
    evLeave el;
    el.m_member_of_session = m_player->getSessionName();
    el.m_triggered_by_session = evt.m_triggered_by_name;

    sendEvent(&el);

    m_player->leaveSession(evt, this);
}


void CCtrlConnection::onSyncInfo(const evSyncStream& evt) {
	m_player->syncInfo(evt, this);
}

void CCtrlConnection::onSetVolume(const evSetVolume& evt) {
	m_player->setVolume(evt, this);
}

void CCtrlConnection::onResetStream(const evResetStream& evt) {
	m_player->resetStream(evt, this);
}

void CCtrlConnection::sendRejectJoin(const evRequestJoin& evt) {
	evJoinRejected rj;

	rj.m_owner_session = m_player->getSessionName();
	rj.m_message = "Client is already member of another seession.";

	sendEvent(&rj);
    // getIoService().post(boost::bind( &IConnectionManager::remove, m_conn_mgr, this));
}

void CCtrlConnection::sendRejectJoin(const evLeave& evt) {
	sendEvent(&evt);
    // getIoService().post(boost::bind( &IConnectionManager::remove, m_conn_mgr, this));
}

void CCtrlConnection::sendEvVolume(const evVolume& evt) {
	sendEvent(&evt);
}

void CCtrlConnection::sendRejectJoin(const evTimeout& evt) {
	// sendEvent(&evt);
    // getIoService().post(boost::bind( &IConnectionManager::remove, m_conn_mgr, this));
}


void CCtrlConnection::sendEvError(const evError& err) {
	sendEvent(&err);
}

void CCtrlConnection::sendEvTimeout(const evTimeout& to) {
	// evTimeout not yet implemented
	// sendEvent(&to);
}

void CCtrlConnection::sendSessionState(const evRequestSessionState &rss) {
    evSessionState ss;
    ss.m_volume = m_player->getVolume();
    ss.m_session_name = m_player->getSessionName();
    //ss.m_mcast_addr = 0;
    ss.m_rtp_unicast_port = m_player->getRTPUnicastPort();
    sendEvent(&ss);
}

