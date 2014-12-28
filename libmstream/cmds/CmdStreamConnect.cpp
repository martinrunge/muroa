/*
 Copyright (c) 2002-2014 "Martin Runge"

 CmdStreamConnect.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CmdStreamConnect.h"
#include "crtppacket.h"

using namespace std;

namespace muroa {


CmdStreamConnect::CmdStreamConnect(CRTPPacket* connectPkt) : CmdStreamBase(E_CONNECT_STREAM) {

}

CmdStreamConnect::CmdStreamConnect(std::string timesrv_addr, int timesrv_port, std::string stream_mcast_grp) : CmdStreamBase(E_CONNECT_STREAM) {
	m_timesrv_addr = timesrv_addr;
	m_timesrv_port = timesrv_port;
	m_stream_mcast_grp = stream_mcast_grp;
}

CmdStreamConnect::~CmdStreamConnect() {

}

string CmdStreamConnect::serialize() {
	string ser_str;

	return ser_str;
}

void CmdStreamConnect::deserialize(std::string ser_str) {

}

} /* namespace muroa */
