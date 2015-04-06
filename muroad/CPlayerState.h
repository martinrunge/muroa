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

#ifndef MUROAD_CPLAYERSTATE_H_
#define MUROAD_CPLAYERSTATE_H_

#include "CConnectionManager.h"
#include <boost/asio.hpp>

class CPlayer;

namespace muroa
{
  class CApp;
  class CSettings;
  class CDnsSdAvahi;
  class CmdStreamReset;
  class CTcpServer;
  class CTimeServiceCtrl;
  class CStreamCtrlConnection;


class CPlayerState {
public:
	CPlayerState(boost::asio::io_service& io_service);
	virtual ~CPlayerState();

	void startPlayer();
	void stopPlayer();

private:
	muroa::CConnectionManager m_conn_mgr;
    muroa::CTcpServer* m_tcp_server;

    muroa::CDnsSdAvahi *m_dnssd;

    boost::asio::io_service& m_io_service;

    CPlayer* m_player;
};

} /* namespace muroa */

#endif /* MUROAD_CPLAYERSTATE_H_ */
