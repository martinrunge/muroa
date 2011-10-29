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
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

#include "CTcpConnection.h"
#include "CConnectionManager.h"
using boost::asio::ip::tcp;

#ifndef CTCPSERVER_H_
#define CTCPSERVER_H_

class CTcpServer : private boost::noncopyable
{
public:
	CTcpServer(boost::asio::io_service& io_service);
	virtual ~CTcpServer();

private:
  void start_accept();
  void handle_accept(CTcpConnection::pointer new_connection, const boost::system::error_code& error);

  tcp::acceptor m_acceptor;
  CConnectionManager m_connectionManager;
  log4cplus::Logger m_logger;
};

#endif /* CTCPSERVER_H_ */
