/***************************************************************************
 *
 *   CTcpConnection.h
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
#include <vector>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

#ifndef CTCPCONNECTION_H_
#define CTCPCONNECTION_H_

using boost::asio::ip::tcp;
using namespace boost;

namespace muroa {

class CTcpConnection
{
public:
  virtual ~CTcpConnection();

  static CTcpConnection* create(boost::asio::io_service& io_service) {
    return new CTcpConnection(io_service);
  }

  tcp::socket& socket() {
    return m_socket;
  }

  virtual void start();
  virtual void stop();

  virtual void setNonBlocking(bool mode = true);

  virtual void onClose();

  tcp::endpoint remoteEndpoint();
  std::string remoteEndpointStr();

  void writeData( const char* buffer, int length );

  inline boost::asio::io_service& getIoService() { return m_socket.get_io_service(); };

protected:
  virtual void dataReceived( boost::array<char, 8192> buffer, int length);
  CTcpConnection(boost::asio::io_service& io_service);

private:

  void start_read();

  void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

  tcp::socket m_socket;

  /// Buffer for incoming data.
  boost::array<char, 8192> m_buffer;

  log4cplus::Logger m_logger;

};

}

#endif /* CTCPCONNECTION_H_ */
