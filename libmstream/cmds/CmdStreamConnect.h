/*
 Copyright (c) 2002-2014 "Martin Runge"

 CmdStreamConnect.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CMDS_CMDSTREAMCONNECT_H_
#define LIBMSTREAM_CMDS_CMDSTREAMCONNECT_H_

#include "CmdStreamBase.h"
#include <boost/asio.hpp>
#include <string>

class CRTPPacket;

namespace muroa {

class CmdStreamConnect: public CmdStreamBase {
public:
	CmdStreamConnect(CRTPPacket* connectPkt);
	CmdStreamConnect(std::string timesrv_addr, int timesrv_port, std::string stream_mcast_grp);
	virtual ~CmdStreamConnect();

	boost::asio::ip::address getAddress();
	void setAddress(boost::asio::ip::address addr);

	std::string serialize();
	void deserialize(std::string json_str);

private:

	// time service
	// IP address
	// stream server
	// IP multicast group

	std::string m_timesrv_addr;
	int m_timesrv_port;
	std::string m_stream_mcast_grp;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CMDS_CMDSTREAMCONNECT_H_ */
