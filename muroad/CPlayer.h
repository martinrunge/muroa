/*
 Copyright (c) 2002-2014 "Martin Runge"

 CPlayerState.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef MUROAD_CPLAYER_H_
#define MUROAD_CPLAYER_H_

#include "CCtrlConnectionMgr.h"
#include <cmds/CmdStream.h>
#include <boost/asio.hpp>

class CMediaStreamConnection;
class CCtrlConnection;

namespace muroa
{
  class CApp;
  class CSettings;
  class CDnsSdAvahi;
  class CmdStreamReset;
  class CTcpServer;
  class CTimeServiceCtrl;
  class CStreamCtrlConnection;


class CPlayer {
public:
	CPlayer(boost::asio::io_service& io_service);
	virtual ~CPlayer();

	int requestJoinSession(std::string name, CCtrlConnection* ctrlConn);
	int requestLeaveSession(CCtrlConnection* ctrlConn);

	bool mayJoinSession(const evRequestJoin& rj, CCtrlConnection* ctrlConn);
	int becomeSessionMember(const evRequestJoin& evt, CCtrlConnection* ctrlConn);

	int onCloseCtrlConn(CCtrlConnection* ctrlConn);

	int leaveSession(const evRequestLeave& evt, CCtrlConnection* ctrlConn);

    void syncInfo(const evSyncStream& evt, CCtrlConnection* ctrlConn);
	void setVolume(const evSetVolume &evt, CCtrlConnection *ctrlConn);
    void reportVolume(const boost::system::error_code& ec);

	void resetStream(const evResetStream& evt, CCtrlConnection* ctrlConn);


	void setupMediaStreamConn(  boost::asio::ip::address mcast_addr, int timesrv_port );
	void shutdownMediaStreamConn();

	const bool multicastSupported() const;
	const int getRTPUnicastPort() const;
	int getVolume();
	boost::asio::ip::address getSessionServer();


	const CCtrlConnection* getSessionCtrlConn() const {
		return m_session_ctrl_conn;
	}

	const std::string getSessionName() const {
		return m_session_name;
	}

private:
	bool m_active;

	CCtrlConnectionMgr m_conn_mgr;
    muroa::CTcpServer* m_tcp_server;

    muroa::CDnsSdAvahi *m_dnssd;

    boost::asio::io_service& m_io_service;

    CMediaStreamConnection* m_media_stream_conn;

    std::string m_session_name;
    boost::asio::ip::address m_mcast_addr;
    int m_timesrv_port;
    int m_current_volume;

	boost::asio::deadline_timer m_report_volume_timer;

	CCtrlConnection* m_session_ctrl_conn;
};

} /* namespace muroa */

#endif /* MUROAD_CPLAYER_H_ */
