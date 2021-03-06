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
#include <IConnectionManager.h>
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
	void setConnectionMgr(muroa::IConnectionManager* conn_mgr) { m_conn_mgr = conn_mgr; };

	void start();
	void shutdown();

	void onClose();
	void onSetup();
	void onShutdown();

	bool onEvent(muroa::CmdStreamBase* ev);

	void dataReceived( boost::array<char, 8192> buffer, int length);

	// IClientSMActions
	void sendClientState();
	void sendSessionState(const evRequestSessionState& rss);
	void confirmJoinRequest(const evRequestJoin& evt);
	bool mayJoinSession(const evRequestJoin& rj);
	void becomeSessionMember(const evRequestJoin& evt);
	void sendEvSessionError(const muroa::evSessionError&);
	void leaveSession(const evRequestLeave& evt);

    void onSyncInfo(const evSyncStream& evt);
	void onSetVolume(const evSetVolume& evt);
	void onResetStream(const evResetStream& evt);
	void onGetSMState(const evGetSMState&);
	void onBecomeMediaProvider(const evBecomeMediaProvider& evt);
    void onRevokeMediaProvider(const evRevokeMediaProvider& evt);

	// send events to server, triggered by statemachine
	void sendRejectJoin(const evRequestJoin& evt);
	void sendRejectJoin(const evLeave& evt);
	void sendRejectJoin(const evTimeout& evt);
	void sendSMState(const evSMState&);

	void sendEvError(const evError& err);
	void sendEvTimeout(const evTimeout& to);
	void sendEvVolume(const evVolume& evv);
    void sendEvt(const CmdStreamBase& evt);

private:
	CCtrlConnection(boost::asio::io_service& io_service);
	void onDataToSend(const char* data, int len);

	muroa::CStreamClientSM m_clnt_sm;
	muroa::CPlayer* m_player;
	muroa::IConnectionManager* m_conn_mgr;
};

#endif /* MUROAD_CCTRLCONNECTION_H_ */
