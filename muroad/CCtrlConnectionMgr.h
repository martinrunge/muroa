/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCtrlConnectionMgr.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef MUROAD_CCTRLCONNECTIONMGR_H_
#define MUROAD_CCTRLCONNECTIONMGR_H_

#include <IConnectionManager.h>
#include "CTcpConnection.h"

#include <set>



namespace muroa {
  class CPlayerState;
  class CTcpConnection;
}

class CCtrlConnectionMgr: public muroa::IConnectionManager {
public:
	CCtrlConnectionMgr(muroa::CPlayerState* ps);
	virtual ~CCtrlConnectionMgr();
    /// Add the specified connection to the manager and start it.
	void add(muroa::CTcpConnection* c);

	/// Stop the specified connection.
	void remove(muroa::CTcpConnection* c);

	/// Stop all connections.
	void removeAll();

	std::set<muroa::CTcpConnection*> getConnections();

private:
    /// The managed connections.
	std::set<muroa::CTcpConnection*> m_connections;

	muroa::CPlayerState* m_player_state;

};

#endif /* MUROAD_CCTRLCONNECTIONMGR_H_ */
