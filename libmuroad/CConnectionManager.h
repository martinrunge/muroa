/***************************************************************************
 *
 *   CConnectionManager.h
 *
 *   This file is part of Asiotest                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
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

#ifndef CCONNECTIONMANAGER_H_
#define CCONNECTIONMANAGER_H_

#include <boost/noncopyable.hpp>
#include "CTcpConnection.h"

#include <set>
namespace muroa {

class CConnectionManager : private boost::noncopyable {
public:
	CConnectionManager();
	virtual ~CConnectionManager();

	  /// Add the specified connection to the manager and start it.
	  void start(CTcpConnection* c);

	  /// Stop the specified connection.
	  void stop(CTcpConnection* c);

	  /// Stop all connections.
	  void stopAll();

	  std::set<CTcpConnection*> getConnections();

	private:
	  /// The managed connections.
	  std::set<CTcpConnection*> m_connections;

};
}
#endif /* CCONNECTIONMANAGER_H_ */
