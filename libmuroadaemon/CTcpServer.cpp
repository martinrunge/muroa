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
#include "avahi/CDnsSdAvahi.h"

using namespace std;
using namespace log4cplus;

namespace muroa {

CTcpServer::CTcpServer(boost::asio::io_service& io_service, IConnectionManager* cm, CApp* app, factory_ptr_t connection_factory)

                     : m_acceptor(io_service),
                       m_connectionManager(cm),
	                   m_logger(Logger::getInstance("main")),
	                   m_app(app),
 	                   m_connection_factory(connection_factory)


{
	CSettings& settings = m_app->settings();

	boost::asio::ip::tcp protocol = tcp::v4();
	if( settings.ipversion() == 6 ) {
		protocol = tcp::v6();
	}

	m_acceptor.open(protocol);
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

	tcp::endpoint endpoint;

	unsigned short portNr = settings.port();
	bool found = false;
	while( !found ) {
		endpoint = tcp::endpoint(protocol, portNr);
		boost::system::error_code ec;
		m_acceptor.bind(endpoint, ec);
		int val = ec.value();
		if( val == EADDRINUSE ) {
			portNr++;
		}
		else {
			found = true;
		}

	}
	settings.setPort(portNr);

	m_dnssd = new CDnsSdAvahi(io_service, settings.serviceName(), portNr, settings.serviceType());
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CApp::serviceChanged, app));


	m_acceptor.listen();
	start_accept();
}

CTcpServer::~CTcpServer() {
	delete m_dnssd;
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
