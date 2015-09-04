/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCtrlConnectionMgr.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include <CCtrlConnectionMgr.h>
#include "CCtrlConnection.h"
#include "CPlayer.h"

using namespace muroa;

CCtrlConnectionMgr::CCtrlConnectionMgr(muroa::CPlayer* pf) : m_player_frame(pf) {

}

CCtrlConnectionMgr::~CCtrlConnectionMgr() {
	removeAll();
}

/// Add the specified connection to the manager and start it.
void CCtrlConnectionMgr::add(CTcpConnection* c) {
	  CCtrlConnection* cc = reinterpret_cast<CCtrlConnection*>(c);
	  cc->setPlayerStatePtr(m_player_frame);
	  m_connections.insert(c);
	  c->start();
}

/// Stop the specified connection.
void CCtrlConnectionMgr::remove(CTcpConnection* c) {
	  CCtrlConnection* cc = reinterpret_cast<CCtrlConnection*>(c);
	  c->stop();
	  m_connections.erase(c);

}

/// Stop all connections.
void CCtrlConnectionMgr::removeAll() {
	  std::for_each(m_connections.begin(), m_connections.end(), boost::bind(&CTcpConnection::stop, _1));
	  m_connections.clear();
}

std::set<CTcpConnection*> CCtrlConnectionMgr::getConnections() {
	return m_connections;
}
