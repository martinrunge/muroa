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
#include "CStreamServer.h"
#include "CStreamCtrlConnection.h"
#include "CStreamConnection.h"

#include "cmds/CmdStreamReset.h"

#include <CApp.h>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>

#include <boost/asio.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace muroa;
using namespace log4cplus;

namespace muroa {

CStreamServer::CStreamServer(boost::asio::io_service& io_service, int timeServerPort, int session_id, int transport_buffer_size_in_ms) :
                             CMediaStreamProvider(io_service, session_id, transport_buffer_size_in_ms),
                             m_time_server_port(timeServerPort),
                             m_io_service(io_service)
{
}

CStreamServer::~CStreamServer()
{
}



void CStreamServer::reportClientState(muroa::CStreamCtrlConnection* conn, const muroa::CmdStreamBase* evt) {
	// evt may be of type evClientState or evLeave
}




/** CStreamServer::addClient(CIPv4Address* addr)
 *
 * \return 0 if client was successfully added, 1 if client was already in the list and -1 if connection failed
 *
 **/
int CStreamServer::addClient(bip::tcp::endpoint endp, const std::string& name)
{
//    set<CStreamCtrlConnection*>::iterator iter;
//    for(iter = m_joined_connections.begin(); iter != m_joined_connections.end(); iter++ ) {
//      bip::tcp::endpoint known_endp = (*iter)->remote_endpoint();
//      if( known_endp == endp ) {
//          return 1;
//      }
//    }

	// checkEndp(endp);

    set<CStreamCtrlConnection*>::iterator iter2;
    for(iter2 = m_sessionless_connections.begin(); iter2 != m_sessionless_connections.end(); iter2++ ) {
      bip::tcp::endpoint known_endp = (*iter2)->remote_endpoint();
      if( known_endp == endp ) {
          return 1;
      }
    }

   	CStreamCtrlConnection* conn = new CStreamCtrlConnection(name, this, m_io_service);
   	boost::system::error_code ec;
  	conn->connect(endp, ec);
   	if(ec) {
   		LOG4CPLUS_ERROR(CApp::logger(), "could not establish control connection to render client " << name << " (" << endp << ")" );
   		delete conn;
   		return -1;
   	}
   	else {
   		conn->setup();
   		// conn->joinSession(0, "place session name here", bip::address() );
   		m_sessionless_connections.insert(conn);
   		return 0;
   	}
}

//void CStreamServer::adjustReceiverList(std::vector<ServDescPtr> receivers)
//{
//    list<CStreamCtrlConnection*>::iterator iter;
//    for(iter = m_joined_connections.begin(); iter != m_joined_connections.end(); iter++ ) {
//
//        bool found = false;
//        for(int i=0; i < receivers.size(); i++) {
//            string servicename = receivers[i]->getServiceName();
//            if( servicename.compare( (*iter)->getServiceName() ) == 0 ) {
//                found = true;
//                receivers.erase(receivers.begin() + i);
//                break;
//            }
//        }
//        if( found = false) {
//            removeClient(iter);
//        }
//    }
//    // add oaa receivers left in receivers list
//    for(int i=0; i < receivers.size(); i++) {
//        ServDescPtr srv_desc_ptr = receivers[i];
//        bip::tcp::endpoint endp(bip::address::from_string(srv_desc_ptr->getHostName()), srv_desc_ptr->getPortNr() );
//        addClient(endp, srv_desc_ptr->getServiceName());
//    }
//}

/*!
    \fn CStreamServer::removeClient(CIPv4Address* addr);
 */


/** remove client irrespective of it is a session member or not
 *  called when tcp ctrl connection was closed by the other end.
 */
void CStreamServer::removeClient(muroa::CStreamCtrlConnection* connPtr) {
	// first check, if it is a session member
	int num_removed = removeJoinedConnection(connPtr);
	if(num_removed > 0) {
		// that client was part of the session
		m_sessionless_connections.insert(connPtr);

	}

	removeSessionlessConnection(connPtr);
}




void CStreamServer::removeClient(const string& name)
{
    set<CStreamCtrlConnection*>::iterator iter;
    for(iter = m_sessionless_connections.begin(); iter != m_sessionless_connections.end(); iter++ ) {
        CStreamCtrlConnection* sc = *iter;
        if( name.compare( sc->getServiceName() ) == 0 ) {
            removeClient(iter);
            iter--;
        }
    }
}

int CStreamServer::removeSessionlessConnection(muroa::CStreamCtrlConnection* connPtr) {
	int num_removed = 0;
    set<CStreamCtrlConnection*>::iterator iter;
    for(iter = m_sessionless_connections.begin(); iter != m_sessionless_connections.end(); iter++ ) {
        CStreamCtrlConnection* sc = *iter;
        if( *iter == connPtr ) {
            removeClient(iter);
            num_removed++;
        }
    }
    return num_removed;
}



void CStreamServer::removeClient(set<CStreamCtrlConnection*>::iterator iter) {
	m_sessionless_connections.erase(iter);
}


void CStreamServer::clientRejectedSessionMember( muroa::CStreamCtrlConnection* conn, const muroa::evJoinRejected* evt) {
}

void CStreamServer::clientBecameSessionMember( muroa::CStreamCtrlConnection* conn, const muroa::evSessionState* evt) {
	// remove conn from the list of connections not associated with a session
	int num_ptr = removeSessionlessConnection(conn);
	if(num_ptr > 0) {
		// it was found in the list of connections not associated with a session
		addJoinedConnection(conn);
	}
}

void CStreamServer::clientLeftSession(muroa::CStreamCtrlConnection* conn, const muroa::evLeave* evt) {
	removeJoinedConnection(conn);
	m_sessionless_connections.insert(conn);
}

} // namespace muroa
