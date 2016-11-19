/*
 Copyright (c) 2002-2015 "Martin Runge"

 CUDPSocket.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CUDPSOCKET_H_
#define LIBMSTREAM_CUDPSOCKET_H_
#include <boost/asio.hpp>

namespace muroa {

class CUDPSocket {
public:
	CUDPSocket();
	CUDPSocket(boost::asio::io_service& io_service);
	CUDPSocket(boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint default_endp, short default_port);
	virtual ~CUDPSocket();

	virtual int  send(const char* buffer, const int len);
	virtual int  recv(char* buffer, const int buflen);

private:
  boost::asio::ip::udp::endpoint m_default_endpoint;
  // boost::asio::ip::udp::socket m_socket;
  short m_default_port;

};

}

#endif /* LIBMSTREAM_CUDPSOCKET_H_ */
