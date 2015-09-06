/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCtrlConnection.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef MUROAD_CCTRLCONNECTION_H_
#define MUROAD_CCTRLCONNECTION_H_

#include <CTcpConnection.h>
#include <ctrlrpcxml/CStreamCtrlXml.h>
#include <CStreamClientSM.h>
#include <IClientSMActions.h>

namespace muroa {
  class CPlayer;
}

class CCtrlConnection: public muroa::CTcpConnection, public muroa::IClientSMActions, public muroa::CStreamCtrlXml {
public:
	static CCtrlConnection* create(boost::asio::io_service& io_service) {
	    return new CCtrlConnection(io_service);
	}
	virtual ~CCtrlConnection();

	void setPlayer(muroa::CPlayer* ps) { m_player = ps; };

	void start();

	void onSetup();
	void onShutdown();

	bool onEvent(muroa::CmdStreamBase* ev);

	void dataReceived( boost::array<char, 8192> buffer, int length);

	// IClientSMActions
	void sendClientState();

private:
	CCtrlConnection(boost::asio::io_service& io_service);
	void onDataToSend(const char* data, int len);

	muroa::CStreamClientSM m_clnt_sm;
	muroa::CPlayer* m_player;
};

#endif /* MUROAD_CCTRLCONNECTION_H_ */
