/*
Copyright (c) 2002-2014 "Martin Runge"

cmds.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CMDS_CMDSTREAM_H_
#define LIBMSTREAM_CMDS_CMDSTREAM_H_

#include "CmdStreamBase.h"

#include <boost/asio.hpp>

namespace muroa {

class evClientState: public CmdStreamBase {
public:
	bool operator==(const evClientState& rhs) {
		return  m_member_of_session.compare( rhs.m_member_of_session ) == 0 &&
				m_session_srv == rhs.m_session_srv &&
				m_current_volume == rhs.m_current_volume;
	}

	std::string m_member_of_session;
	boost::asio::ip::address m_session_srv;
	int m_current_volume;

	static const std::string ev_name;
};

class evRequestJoin: public CmdStreamBase {
public:
	bool operator==(const evRequestJoin& rhs) {
		return  m_session_name.compare( rhs.m_session_name ) == 0 &&
				m_mcast_addr == rhs.m_mcast_addr &&
				m_timesrv_port == rhs.m_timesrv_port;
	}
	std::string m_session_name;
	boost::asio::ip::address m_mcast_addr;
	uint32_t m_timesrv_port;

	static const std::string ev_name;
};


class evJoinAccepted: public CmdStreamBase {
public:
	bool operator==(const evJoinAccepted& rhs) {
		return  m_former_session.compare( rhs.m_former_session ) == 0;
	}
	std::string m_former_session;

	static const std::string ev_name;
};

class evJoinRejected: public CmdStreamBase {
public:
	bool operator==(const evJoinRejected& rhs) {
		return  m_owner_session.compare( rhs.m_owner_session ) == 0 &&
				m_message.compare(rhs.m_message) == 0;
	}
	std::string m_owner_session;
	std::string m_message;

	static const std::string ev_name;
};

class evLeave: public CmdStreamBase {
public:
	bool operator==(const evLeave& rhs) {
		return  m_triggered_by_session.compare( rhs.m_triggered_by_session ) == 0;
	}
	std::string m_triggered_by_session;

	static const std::string ev_name;
};

class evGetSessionState: public CmdStreamBase {
public:
	bool operator==(const evGetSessionState& rhs) {
		return  m_session_name.compare( rhs.m_session_name ) == 0;
	}

	std::string m_session_name;

	static const std::string ev_name;
};

class evSessionState: public CmdStreamBase {
public:
	bool operator==(const evSessionState& rhs) {
		return  m_session_name.compare( rhs.m_session_name ) == 0 &
				m_mcast_addr == rhs.m_mcast_addr &&
				m_timesrv_port == rhs.m_timesrv_port &&
				m_rtp_unicast_port == rhs.m_rtp_unicast_port &&
				m_volume == rhs.m_volume;
	}

	std::string m_session_name;
	boost::asio::ip::address m_mcast_addr;
	uint32_t m_timesrv_port;
	uint32_t m_rtp_unicast_port;
	uint32_t m_volume;

	static const std::string ev_name;
};


class evResetStream: public CmdStreamBase {
public:
	bool operator==(const evResetStream& rhs) {
		return  m_ssrc == rhs.m_ssrc &&
				m_rtp_ts == rhs.m_rtp_ts &&
				m_media_clock_pts == rhs.m_media_clock_pts;
	}
	uint32_t m_ssrc;
	uint32_t m_rtp_ts;
	uint64_t m_media_clock_pts;

	static const std::string ev_name;
};

class evSyncStream: public CmdStreamBase {
public:
	bool operator==(const evSyncStream& rhs) {
		return  m_ssrc == rhs.m_ssrc &&
				m_rtp_ts == rhs.m_rtp_ts &&
				m_media_clock_pts == rhs.m_media_clock_pts;
	}

	uint32_t m_ssrc;
	uint32_t m_rtp_ts;
	uint64_t m_media_clock_pts;

	static const std::string ev_name;
};

class evSetVolume: public CmdStreamBase {
public:
	bool operator==(const evSetVolume& rhs) {
		return  m_ssrc == rhs.m_ssrc &&
				m_volume == rhs.m_volume;
	}

	uint32_t m_ssrc;
	uint32_t m_volume;

	static const std::string ev_name;
};


class evAck: public CmdStreamBase {
public:
	bool operator==(const evAck& rhs) {
		return  m_cmd_id == rhs.m_cmd_id;
	}
	static const std::string ev_name;
};

class evError: public CmdStreamBase {
public:
	bool operator==(const evError& rhs) {
		return  m_error_msg.compare(rhs.m_error_msg) == 0;
	}
	std::string m_error_msg;

	static const std::string ev_name;
};


// internal event only used by state machine, not serialized and sent over network
class evTimeout {

};

struct evCmdSent {

};


}
#endif /* LIBMSTREAM_CMDS_CMDSTREAM_H_ */
