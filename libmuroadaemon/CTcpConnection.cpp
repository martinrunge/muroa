/***************************************************************************
 *
 *   CTcpConnection.cpp
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

#include "CTcpConnection.h"
#include "CApp.h"
#include <iostream>
#include <log4cplus/loggingmacros.h>


using namespace std;
using namespace log4cplus;

namespace muroa {

CTcpConnection::CTcpConnection(boost::asio::io_service& io_service) : m_socket(io_service) {
}

CTcpConnection::~CTcpConnection() {
    LOG4CPLUS_INFO(CApp::logger(), "closed connection to: " << remoteEndpointStr());
}


void CTcpConnection::start() {
    LOG4CPLUS_INFO(CApp::logger(), "new connection accepted from: " << remoteEndpointStr());

	start_read();
}

void CTcpConnection::stop() {
    LOG4CPLUS_INFO(CApp::logger(), "stopping connection to: " << remoteEndpointStr() << " Closing socket now.");
	m_socket.close();
}

void CTcpConnection::setNonBlocking(bool mode) {
#ifdef NEW_BOOST_ASIO
	m_socket.non_blocking(true);
#else
	boost::asio::socket_base::non_blocking_io command(mode);
	m_socket.io_control(command);
#endif

//	boost::asio::socket_base::send_buffer_size option(2048);
//	m_socket.set_option(option);

}

void CTcpConnection::onClose() {

}
tcp::endpoint CTcpConnection::localEndpoint() {
	return m_socket.local_endpoint();
}

std::string CTcpConnection::localEndpointStr() {
	tcp::endpoint local = m_socket.local_endpoint();
	boost::asio::ip::address addr = local.address();
	return addr.to_string();
}

tcp::endpoint CTcpConnection::remoteEndpoint() {
	return m_socket.remote_endpoint();
}

std::string CTcpConnection::remoteEndpointStr() {
	tcp::endpoint remote = m_socket.remote_endpoint();
	boost::asio::ip::address addr = remote.address();
	return addr.to_string();
}

void CTcpConnection::writeData( const char* buffer, int length) {
	m_socket.async_send(asio::buffer(buffer, length),
                         boost::bind(&CTcpConnection::handle_write, this,
                                     asio::placeholders::error,
                                     asio::placeholders::bytes_transferred));
	return;
}

void CTcpConnection::dataReceived( boost::array<char, 8192> /*buffer*/, int /*length*/) {
	return;
}


void CTcpConnection::handle_write(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        LOG4CPLUS_ERROR(CApp::logger(), "error in handle_write:  " << error.message());
        onClose();
    }
    else {
    	// LOG4CPLUS_DEBUG(CApp::logger(), "handle_write: " << bytes_transferred << " Bytes transferred");
    }
}

void CTcpConnection::start_read() {
	m_socket.async_read_some(asio::buffer(m_buffer),
                             boost::bind(&CTcpConnection::handle_read, this,
                                         asio::placeholders::error,
                                         asio::placeholders::bytes_transferred));
}


void CTcpConnection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
    	dataReceived(m_buffer, bytes_transferred);
    	start_read();
    }
    else {
    	if(error == boost::asio::error::eof ) {
    		// connection was closed
    		onClose();

    	}
    	LOG4CPLUS_ERROR(CApp::logger(), "error in handle_read:  " << error.message());
    	// delete this;
    }
}

}
