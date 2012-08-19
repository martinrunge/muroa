/***************************************************************************
 *
 *   CTcpServer.h
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


#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

#include "CTcpConnection.h"
#include "IConnectionManager.h"

using boost::asio::ip::tcp;

#ifndef CTCPSERVER_H_
#define CTCPSERVER_H_

namespace muroa {

class CApp;
class CDnsSdAvahi;

typedef CTcpConnection*(*factory_ptr_t)(boost::asio::io_service& io_service);

class CTcpServer : private boost::noncopyable
{
public:
	CTcpServer(boost::asio::io_service& io_service, IConnectionManager* cm, CApp* app, factory_ptr_t connection_factory);
	virtual ~CTcpServer();
	IConnectionManager* getConnctionManager();

	void setConnectionFactory( factory_ptr_t connection_factory );
	factory_ptr_t getConnectionFactory(void);

private:
  void start_accept();
  void handle_accept(CTcpConnection* new_connection, const boost::system::error_code& error);

  tcp::acceptor m_acceptor;
  IConnectionManager* m_connectionManager;
  factory_ptr_t m_connection_factory;
  log4cplus::Logger m_logger;

  CApp* m_app;
};

}
#endif /* CTCPSERVER_H_ */
