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


using namespace std;
using namespace log4cplus;


CTcpServer::CTcpServer(boost::asio::io_service& io_service) : m_acceptor(io_service), m_logger(Logger::getInstance("main")) {
	tcp::endpoint endpoint(tcp::v4(), 1356);
	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();

	start_accept();
}

CTcpServer::~CTcpServer() {
	// TODO Auto-generated destructor stub
}

void CTcpServer::start_accept() {
  CTcpConnection::pointer new_connection = CTcpConnection::create(m_acceptor.get_io_service());

  m_acceptor.async_accept(new_connection->socket(),
                         boost::bind(&CTcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void CTcpServer::handle_accept(CTcpConnection::pointer new_connection, const boost::system::error_code& error) {
  if (!error) {
	  m_connectionManager.start(new_connection);
  }
  start_accept();
}
