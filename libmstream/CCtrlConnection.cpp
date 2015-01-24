/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCtrlConnection.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CCtrlConnection.h"

namespace muroa {

CCtrlConnection::CCtrlConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service) {

}

CCtrlConnection::~CCtrlConnection() {

}

void CCtrlConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	// m_rpc->newData(buffer.data(), length);
}

} /* namespace muroa */
