/***************************************************************************
 *
 *   CTcpServer.cpp
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

#include "CTcpServer.h"
#include "CApp.h"

using namespace std;
using namespace log4cplus;

using boost::asio::ip::tcp;

namespace muroa {

CTcpServer::CTcpServer(boost::asio::io_service& io_service, IConnectionManager* cm, tcp::endpoint& endp, factory_ptr_t connection_factory)

                     : m_acceptor(io_service),
                       m_connectionManager(cm),
 	                   m_connection_factory(connection_factory)


{
	m_acceptor.open(endp.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

	bool found = false;
	while( !found ) {
		boost::system::error_code ec;
		m_acceptor.bind(endp, ec);
		int val = ec.value();
		if( val == EADDRINUSE ) {
			endp.port(endp.port() + 1);
		}
		else {
			found = true;
			endp = m_acceptor.local_endpoint();
		}
	}

	m_acceptor.listen();
	start_accept();
}

CTcpServer::~CTcpServer() {
}

void CTcpServer::setConnectionFactory( factory_ptr_t connection_factory ) {
	m_connection_factory = connection_factory;
}

factory_ptr_t CTcpServer::getConnectionFactory(void) {
	return m_connection_factory;
}


IConnectionManager* CTcpServer::getConnctionManager() {
	return m_connectionManager;
}

void CTcpServer::start_accept() {
  CTcpConnection* new_connection = m_connection_factory(m_acceptor.get_io_service());

  m_acceptor.async_accept(new_connection->socket(),
                         boost::bind(&CTcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void CTcpServer::handle_accept(CTcpConnection* new_connection, const boost::system::error_code& error) {
  if (!error) {
	  new_connection->setNonBlocking();
	  new_connection->start();
	  m_connectionManager->add(new_connection);
  }
  start_accept();
}

}
