/*
Copyright (c) 2002-2014 "Martin Runge"

server.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_EXAMPLES_SERVER_H_
#define LIBMSTREAM_EXAMPLES_SERVER_H_

#include <iostream>
#include <cstdio>
#include <getopt.h>
#include <vector>
#include <string>

#include <boost/asio.hpp>
#include <CStreamServer.h>
#include "CStreamDecoder.h"

namespace bip=boost::asio::ip;

class CWSMsgHandler;

class CppServer :public CStreamServer {
public:
	CppServer(boost::asio::io_service& io_service, std::vector<std::string> clients, int timeServerPort, int sessionID );
	~CppServer();

	// void setWSSrv(muroa::WSSrv *wssrv);
	void setWSMsgHandler(CWSMsgHandler* msg_handler) { m_ws_msg_handler = msg_handler; };

	void addClientByName(std::string serviceName);
	void removeClient(std::string serviceName);

	void playStream(std::string url);
	void stopStream();

	void clientRejectedSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evJoinRejected* evt);
	void clientBecameSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evSessionState* evt);
	void clientLeftSession(muroa::CStreamCtrlConnection* conn, const muroa::evLeave* evt);

	void reportError(muroa::CStreamCtrlConnection* conn, const evJoinRejected* evt);

	void reportClientState(muroa::CStreamCtrlConnection* conn, const muroa::CmdStreamBase* evt);
	void reportProgress( int posInSecs, int durationInSecs);

	void onClientAppeared(ServDescPtr srvPtr);
	void onClientDisappeared(ServDescPtr srvPtr);
	void onClientChanged();

	const std::vector<CRenderClientDesc> getRenderClients() const { return m_rcs; };

protected:
	bool endpointOfService(std::string serviceName, bip::tcp::endpoint& endp);

	void addClientToSelected(const string& serviceName);
	void removeClientFromSelected(const string& serviceName);
	void storeClientList();

private:

	boost::asio::io_service& m_io_service;
	CStreamDecoder          *m_decoder;

	// muroa::WSSrv            *m_ws_srv;
	CWSMsgHandler            *m_ws_msg_handler;

	// the list of render client currently active and part of the session
	vector<ServDescPtr>      m_clients;

	// the list of render clients that should be part of the session. If any of them
	// goes online, it will be automatically added to the session. it was never removed
	// from the session, but disappeared from the network.
	vector<std::string>      m_selected_clients;

	// list of clients as shown in the GUI. Inlcudes offline clients and clients that are not member of the session.
	vector<CRenderClientDesc> m_rcs;
};




#endif /* LIBMSTREAM_EXAMPLES_SERVER_H_ */
