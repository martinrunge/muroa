/***************************************************************************
 *
 *   CConnectionManager.cpp
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

#include "CConnectionManager.h"

using namespace std;

namespace muroa {

CConnectionManager::CConnectionManager() {
}

CConnectionManager::~CConnectionManager() {
	removeAll();
}

/// Add the specified connection to the manager and start it.
void CConnectionManager::add(CTcpConnection* c) {
	  m_connections.insert(c);
	  c->start();
}

/// Stop the specified connection.
void CConnectionManager::remove(CTcpConnection* c) {
	  m_connections.erase(c);
	  c->stop();

}

/// Stop all connections.
void CConnectionManager::removeAll() {
	  std::for_each(m_connections.begin(), m_connections.end(), boost::bind(&CTcpConnection::stop, _1));
	  m_connections.clear();
}

std::set<CTcpConnection*> CConnectionManager::getConnections() {
	return m_connections;
}

}
