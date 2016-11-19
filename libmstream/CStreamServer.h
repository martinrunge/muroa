/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CSTREAMSERVER_H
#define CSTREAMSERVER_H

/**
Class provides a server for a stream.

@author Martin Runge
*/

#include <iostream>
#include <set>
#include <list>
#include <string>
#include <sys/time.h>
#include "avahi/CServiceDesc.h"

// #include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
// #include "libsock++.h"
// #include "libdsaudio.h"

#include "CStreamCtrlConnection.h"
#include "CMediaStreamProvider.h"
#include "CStreamClientDiscovery.h"

#include "CRenderClientDesc.h"

#include "csync.h"
#include "cmutex.h"
#include "crtppacket.h"


#include <log4cplus/logger.h>

class CIPv4Address;

namespace bip=boost::asio::ip;

namespace muroa {

class CStreamServer : public CMediaStreamProvider, public CStreamClientDiscovery {
public:
    /**
     * @brief CStreamServer constructor.
     *
     * Creates a new stream server object. A stream server has a
     * session_id and a stream_id. By calling close() and open(),
     * a new stream_id will be generated.
     *
     * @param[in] session_id: the ID of the session. Increase it if you create a new CStreamServer object.
     * @param[in] transport_buffer_size_in_ms: the size of the client's playback buffer in ms. Aka difference
     * between send timestamp and presentation timestamp.
     */
    CStreamServer(boost::asio::io_service& io_service, std::string session_name, int timeServerPort, int session_id = 1, int transport_buffer_size_in_ms = 1500);
    ~CStreamServer();

	void connectToClient(std::string serviceName);
	void requestJoin(std::string serviceName);
	void requestLeave(std::string serviceName);
	void disconnectFromClient(const std::string& serviceName);


    // void adjustReceiverList(std::vector<muroa::ServDescPtr> receivers);
    __attribute__((deprecated("use connectToClient instead and wait for the onClientState event")))
    virtual int addClient(bip::tcp::endpoint endp, const std::string& name);

	// virtual void removeClientByName(const std::string &name);
    // void removeClient(muroa::CStreamCtrlConnection* connPtr);

	virtual void onClientState(muroa::CStreamCtrlConnection* conn, const muroa::evClientState* evt);

	virtual void onClientRejectedSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evJoinRejected* evt);
	virtual void onClientBecameSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evSessionState* evt);
	virtual void onClientLeftSession(muroa::CStreamCtrlConnection* conn, const muroa::evLeave* evt);

	virtual void onError(muroa::CStreamCtrlConnection* conn, const evJoinRejected* evt);
	virtual void onError(muroa::CStreamCtrlConnection* conn, const evError* evt);

	const std::vector<CRenderClientDesc> getRenderClients() const { return m_rcs; };

protected:
	bool endpointOfService(std::string serviceName, bip::tcp::endpoint& endp);
	bip::tcp::endpoint endpointOfService(const std::string& serviceName);  // throws CUnknownServiceNameException
	muroa::ServDescPtr srvDescOfService(const std::string& serviceName);  // throws CUnknownServiceNameException
	CRenderClientDesc getRenderClientDescByName(const std::string& serviceName);   // throws CUnknownServiceNameException
	CStreamCtrlConnection* getCtrlConnByName(const std::string& serviceName);   // throws CUnknownServiceNameException

	void connectToClient(bip::tcp::endpoint endp, const std::string& serviceName);
	void requestJoin(CStreamCtrlConnection* conn);
	void requestLeave(CStreamCtrlConnection* conn);

	virtual void onClientAppeared(ServDescPtr srvPtr);
	virtual void onClientDisappeared(ServDescPtr srvPtr);

	//void removeClient(set<CStreamCtrlConnection*>::iterator iter);

    const std::string m_session_name;

private:
    int m_time_server_port;

    CMutex m_sessionless_connection_list_mutex;
    set<CStreamCtrlConnection*> m_sessionless_connections;

    boost::asio::io_service& m_io_service;

	// list of clients as shown in the GUI. Inlcudes offline clients and clients that are not member of the session.
	vector<CRenderClientDesc> m_rcs;
};

} // namespace muroa

#endif
