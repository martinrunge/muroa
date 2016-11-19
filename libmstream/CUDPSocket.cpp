/*
 Copyright (c) 2002-2015 "Martin Runge"

 CUDPSocket.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CUDPSocket.h"

namespace muroa {

using namespace boost::asio::ip;

CUDPSocket::CUDPSocket() {

}

CUDPSocket::CUDPSocket(boost::asio::io_service& io_service) //:  m_socket(io_service, udp::endpoint(udp::v4(), 8888)), m_default_port(0)
{

}

CUDPSocket::CUDPSocket(boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint default_endp, short default_port)
//               : m_socket(io_service),
//				 m_default_endpoint(default_endp),
//				 m_default_port(default_port)
{

}

CUDPSocket::~CUDPSocket() {
}

int  CUDPSocket::send(const char* buffer, const int len) {

}

int  CUDPSocket::recv(char* buffer, const int buflen) {

}

} // namespace muroa
