/*
 Copyright (c) 2002-2014 "Martin Runge"

 CPlayerState.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include <CMediaStreamConnection.h>
#include <CPlayer.h>

#include "CApp.h"
#include "CSettings.h"
#include "CTcpServer.h"
#include "CCtrlConnection.h"
#include "avahi/CDnsSdAvahi.h"

#include <string>

#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace muroa;

namespace muroa {

CPlayer::CPlayer(boost::asio::io_service& io_service) :
		m_conn_mgr(this),
		m_tcp_server(0),
		m_io_service(io_service),
		m_media_stream_conn(0),
		m_session_name(),
        m_report_volume_timer(io_service),
		m_session_ctrl_conn(0) {

	boost::asio::ip::tcp protocol = tcp::v4();
	if( CApp::settings().getConfigVal("muroad.useIPv6", "no").compare(string("yes")) == 0) {
		protocol = tcp::v6();
	}
	tcp::endpoint endp = tcp::endpoint(protocol, CApp::settings().getConfigVal("muroad.ControlPort", 5555));
	m_tcp_server = new CTcpServer(io_service, &m_conn_mgr, endp, reinterpret_cast<factory_ptr_t>(&CCtrlConnection::create));

    // CTcpServer searches for a free port to bind to starting at the given port.
	// endp is passed as reference and after return contains the port that was actually was bound.
	CApp::settings().setPersistentVal(string("muroad.ControlPort"), (const int)endp.port());

	string serviceName = CApp::settings().getConfigVal("muroad.ServiceName", "Muroa Streaming Client");
	string serviceType = CApp::settings().getConfigVal("muroad.ServiceType", "_muroad._tcp");
//	string serviceType = CApp::settings().getConfigVal("muroad.ServiceType", "_muroad._udp");

    m_dnssd = new CDnsSdAvahi(io_service, serviceName, endp.port(), serviceType, vector<string>());
//    m_dnssd = new CDnsSdAvahi(io_service, serviceName, 44400, serviceType, vector<string>());
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CApp::serviceChanged, CApp::getInstPtr()));

	m_current_volume = getVolume();

	int max_co_in_ms = CApp::settings().getConfigVal("muroad.MaxClockOffset", 1000);
	m_max_tolerable_clock_offset = CDuration(static_cast<int64_t>(1000000) * max_co_in_ms);
}

CPlayer::~CPlayer() {

	delete m_dnssd;

	delete m_tcp_server;
	if(m_media_stream_conn != 0) {
		delete m_media_stream_conn;
		m_media_stream_conn = 0;
	}
}


void CPlayer::setupMediaStreamConn( boost::asio::ip::address mcast_addr, int timesrv_port) {
	assert(m_media_stream_conn == 0);
	boost::asio::ip::udp::endpoint timesrv_endpoint(m_session_ctrl_conn->remoteEndpoint().address(), timesrv_port);
	m_media_stream_conn = new CMediaStreamConnection(this, mcast_addr, timesrv_endpoint);

    m_current_volume = getVolume();

    evSetVolume evsv;
    evsv.m_ssrc = 0;
    evsv.m_volume = m_current_volume;

    m_media_stream_conn->onSetVolume(evsv);
}

void CPlayer::shutdownMediaStreamConn() {
	if(m_media_stream_conn != 0) {
		delete m_media_stream_conn;
		m_media_stream_conn = 0;
	}
}


/**
 * \return: 0 on success (join request accepted, render client is now member of session)
 *          1 render client was already member of session
 *          -1 render client is member of another session
 *          -2 access denied
 */
int CPlayer::requestJoinSession(std::string name, CCtrlConnection* ctrlConn) {
	if( name.compare(m_session_name) == 0 && m_session_ctrl_conn == ctrlConn ) {
		LOG4CPLUS_INFO(CApp::logger(), "received request to join session " << name << " (" << ctrlConn->remoteEndpoint() << ") while already member of that session");
		return 1;
	}

	if( m_session_name.empty() && m_session_ctrl_conn == 0 ) {
		LOG4CPLUS_INFO(CApp::logger(), "accepting request to join session " << name << " (" << ctrlConn->remoteEndpoint() << ")");

		// setupMediaStreamConn();
		// m_media_stream_conn->getRTPPort();

		// m_session_name = name;
		// m_session_ctrl_conn = ctrlConn;
		return 0;
	}

	LOG4CPLUS_INFO(CApp::logger(), "denying request to join session " << name << " (" << ctrlConn->remoteEndpoint() << ")  because already member of session " << m_session_name << " (" << m_session_ctrl_conn->remoteEndpoint() << ").");
	return -1;
}


int CPlayer::requestLeaveSession(CCtrlConnection* ctrlConn) {
	if( ctrlConn == m_session_ctrl_conn) {
		shutdownMediaStreamConn();
		m_conn_mgr.remove(m_session_ctrl_conn);
		m_session_ctrl_conn = 0;
		m_session_name.clear();
		return 0;
	}
	else {
		return -1;
	}
}

bool CPlayer::mayJoinSession(const evRequestJoin& rj, CCtrlConnection* ctrlConn) {
	if(m_session_name.empty() && m_session_ctrl_conn == 0 ) {
		return true;
	}
	else {
		return false;
	}
}

int CPlayer::becomeSessionMember(const evRequestJoin& evt, CCtrlConnection* ctrlConn) {
	// setup session
	m_session_name = evt.m_session_name;
	m_session_ctrl_conn = ctrlConn;
//    m_mcast_addr = evt.m_mcast_addr;
//    m_timesrv_port = evt.m_timesrv_port;
    setupMediaStreamConn(evt.m_mcast_addr, evt.m_timesrv_port);
}

int CPlayer::leaveSession(const evSessionError& evt) {
	m_session_name = "";
	m_session_ctrl_conn = 0;
	shutdownMediaStreamConn();
}

int CPlayer::leaveSession(const evRequestLeave& evt, CCtrlConnection* ctrlConn) {
	m_session_name = "";
	m_session_ctrl_conn = 0;
	shutdownMediaStreamConn();
}

int CPlayer::onCloseCtrlConn(CCtrlConnection* ctrlConn) {
	if(m_session_ctrl_conn == ctrlConn) {
		shutdownMediaStreamConn();
		m_session_ctrl_conn = 0;
		m_session_name.clear();
	}
	m_conn_mgr.remove(ctrlConn);
}

void CPlayer::syncInfo(const evSyncStream& evt, CCtrlConnection* ctrlConn) {
	if(ctrlConn == m_session_ctrl_conn) {
		m_media_stream_conn->onSyncInfo(evt);
	}
}

void CPlayer::onClockOffset(CDuration theta) {
	if(theta > m_max_tolerable_clock_offset || theta < -m_max_tolerable_clock_offset) {

		ostringstream oss;
		oss << "CPlayer::onClockOffset to server [s]: " << std::fixed << theta.sec();
		LOG4CPLUS_DEBUG(CApp::logger(), oss.str() );

		evSessionError evsesserr;
		evsesserr.m_client_name = m_dnssd->getServiceName();
		evsesserr.m_member_of_session = getSessionName();
		evsesserr.m_error_msg = oss.str();
		evsesserr.m_clock_offset = theta;
		m_session_ctrl_conn->onEvent(&evsesserr);
	}
}

int CPlayer::getVolume() {
    return CApp::settings().getPersistentVal("muroad.Volume", 50 );
}

void CPlayer::setVolume(const evSetVolume &evt, CCtrlConnection *ctrlConn) {
	if(ctrlConn == m_session_ctrl_conn) {
		m_media_stream_conn->onSetVolume(evt);
        m_current_volume = evt.m_volume;

        // asio cancelles any unexpired deadlines automatically
        m_report_volume_timer.expires_from_now( boost::posix_time::seconds(3) );
        m_report_volume_timer.async_wait(boost::bind( &CPlayer::reportVolume, this, boost::asio::placeholders::error ));
	}
}

void CPlayer::reportVolume(const boost::system::error_code& ec) {
    if( ec != boost::asio::error::operation_aborted) {
        CApp::settings().setPersistentVal("muroad.Volume", (const int &) m_current_volume);
        evVolume evvol;
        evvol.m_ssrc = 0;
        evvol.m_volume = m_current_volume;
        m_session_ctrl_conn->onEvent(&evvol);
    }
}


void CPlayer::resetStream(const evResetStream& evt, CCtrlConnection* ctrlConn) {
	if(ctrlConn == m_session_ctrl_conn) {
		m_media_stream_conn->onResetStream(evt);
	}
}

void CPlayer::getSMState(const evGetSMState& evt, CCtrlConnection* ctrlConn) {

}

boost::asio::ip::address CPlayer::getSessionServer() {
	boost::asio::ip::address addr;

	if(m_session_ctrl_conn != 0) {
		addr = m_session_ctrl_conn->remoteEndpoint().address();
	}
	else {
		addr = boost::asio::ip::address::from_string("0.0.0.0");
	}
	return addr;
}

const bool CPlayer::multicastSupported() const {
	if(m_media_stream_conn != 0)
		return m_media_stream_conn->multicastSupported();
	else
		return false;
}

const int CPlayer::getRTPUnicastPort() const {
	if(m_media_stream_conn != 0)
		return m_media_stream_conn->getRTPUnicastPort();
	else
		return 0;
}

} /* namespace muroa */
