 /**************************************************************************
 *                                                                         *
 *   CStreamCtrlParserSM.cpp                                               *
 *                                                                         *
 *   This file is part of libmstream                                       *
 *   Copyright (C) 2014 by Martin Runge <martin.runge@web.de>              *
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

#include "CStreamCtrlParserSM.h"
#include "CUtils.h"

#include <iostream>
#include <assert.h>

using namespace std;
using namespace muroa;
using namespace boost::asio;

namespace muroa {

CStreamCtrlParserSM::CStreamCtrlParserSM()
{
	reset();
}

void CStreamCtrlParserSM::reset() {
	m_concatenate_chars_state = false;
	m_concatenate_dummy.erase();
	// m_command_connection = parent;
	// m_session = session;


	m_xml_parser_state.root_state = XML_ROOT_STATE;
	m_xml_parser_state.open_state = XML_OPEN_STATE_ROOT;
	m_xml_tag_depth = 0;
	m_tmp_cmd_ptr = 0;
	m_tag_unknown_depth = 0;
	m_in_unknown_tag = false;
}

CStreamCtrlParserSM::~CStreamCtrlParserSM()
{
}

void CStreamCtrlParserSM::onXmlVersion() {

}


void CStreamCtrlParserSM::onXmlStartDocument()
{
	std::cerr << "on_start_document()" << std::endl;
}

void CStreamCtrlParserSM::onXmlEndDocument()
{
	std::cerr << "on_end_document()" << std::endl;
}

void CStreamCtrlParserSM::onXmlStartElement(const std::string& name, const char** attributes)
{
	if(name.compare( evClientState::ev_name ) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evClientState* cmd = new evClientState();
		parseCmdID(attributes, cmd);
		parseEvClientStateArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evRequestJoin::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evRequestJoin* cmd = new evRequestJoin();
		parseCmdID(attributes, cmd);
		parseEvRequestJoinArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evRequestLeave::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evRequestLeave* cmd = new evRequestLeave();
		parseCmdID(attributes, cmd);
		parseEvRequestLeaveArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evJoinAccepted::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evJoinAccepted* cmd = new evJoinAccepted();
		parseCmdID(attributes, cmd);
		parseEvJoinAcceptedArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evJoinRejected::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evJoinRejected* cmd = new evJoinRejected();
		parseCmdID(attributes, cmd);
		parseEvJoinRejectedArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evSessionError::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evSessionError* cmd = new evSessionError();
		parseCmdID(attributes, cmd);
		parseEvSessionErrorArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evLeave::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evLeave* cmd = new evLeave();
		parseCmdID(attributes, cmd);
		parseEvLeaveArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evGetSessionState::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evGetSessionState* cmd = new evGetSessionState();
		parseCmdID(attributes, cmd);
		parseEvGetSessionStateArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evSessionState::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evSessionState* cmd = new evSessionState();
		parseCmdID(attributes, cmd);
		parseEvSessionStateArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evResetStream::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evResetStream* cmd = new evResetStream();
		parseCmdID(attributes, cmd);
		parseEvResetStreamArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evSyncStream::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evSyncStream* cmd = new evSyncStream();
		parseCmdID(attributes, cmd);
		parseEvSyncStreamArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evSetVolume::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evSetVolume* cmd = new evSetVolume();
		parseCmdID(attributes, cmd);
		parseEvSetVolumeArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
    } else if( name.compare( evVolume::ev_name) == 0) {
        assert(m_tmp_cmd_ptr == 0);
        evVolume* cmd = new evVolume();
        parseCmdID(attributes, cmd);
        parseEvVolumeArgs(attributes, cmd);
        m_tmp_cmd_ptr = cmd;
    } else if( name.compare( evRequestProv::ev_name) == 0) {
        assert(m_tmp_cmd_ptr == 0);
        evRequestProv* cmd = new evRequestProv();
        parseCmdID(attributes, cmd);
        parseEvRequestProvArgs(attributes, cmd);
        m_tmp_cmd_ptr = cmd;
    } else if( name.compare( evProvAck::ev_name) == 0) {
        assert(m_tmp_cmd_ptr == 0);
        evProvAck* cmd = new evProvAck();
        parseCmdID(attributes, cmd);
        parseEvProvAckArgs(attributes, cmd);
        m_tmp_cmd_ptr = cmd;
    } else if( name.compare( evProvRej::ev_name) == 0) {
        assert(m_tmp_cmd_ptr == 0);
        evProvRej* cmd = new evProvRej();
        parseCmdID(attributes, cmd);
        parseEvProvRejArgs(attributes, cmd);
        m_tmp_cmd_ptr = cmd;
    } else if( name.compare( evRevokeProv::ev_name) == 0) {
        assert(m_tmp_cmd_ptr == 0);
        evRevokeProv* cmd = new evRevokeProv();
        parseCmdID(attributes, cmd);
        parseEvRevokeProvArgs(attributes, cmd);
        m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evAck::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evAck* cmd = new evAck();
		parseCmdID(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	} else if( name.compare( evError::ev_name) == 0) {
		assert(m_tmp_cmd_ptr == 0);
		evError* cmd = new evError();
		parseCmdID(attributes, cmd);
		parseEvErrorArgs(attributes, cmd);
		m_tmp_cmd_ptr = cmd;
	}
	else {
	}
	return;
}

void CStreamCtrlParserSM::onXmlEndElement(const std::string& name)
{
	// std::cerr << "on_end_element()" << std::endl;
	const char **null_ptr( 0 );

	if(	name.compare( evClientState::ev_name     ) == 0 ||
		name.compare( evRequestJoin::ev_name     ) == 0 ||
		name.compare( evRequestLeave::ev_name    ) == 0 ||
		name.compare( evJoinAccepted::ev_name    ) == 0 ||
		name.compare( evJoinRejected::ev_name    ) == 0 ||
		name.compare( evSessionError::ev_name    ) == 0 ||
		name.compare( evLeave::ev_name           ) == 0 ||
		name.compare( evGetSessionState::ev_name ) == 0 ||
		name.compare( evSessionState::ev_name    ) == 0 ||
		name.compare( evResetStream::ev_name     ) == 0 ||
		name.compare( evSyncStream::ev_name      ) == 0 ||
		name.compare( evSetVolume::ev_name       ) == 0 ||
        name.compare( evVolume::ev_name          ) == 0 ||
        name.compare( evRequestProv::ev_name     ) == 0 ||
        name.compare( evProvAck::ev_name         ) == 0 ||
        name.compare( evProvRej::ev_name         ) == 0 ||
        name.compare( evRevokeProv::ev_name      ) == 0 ||
		name.compare( evAck::ev_name             ) == 0 ||
		name.compare( evError::ev_name           ) == 0)
	{
			assert(m_tmp_cmd_ptr != 0);
			bool event_consumed = true;
			try {
				event_consumed = onEvent(m_tmp_cmd_ptr);
			}
			catch(CException ex) {
				;
			}
			if(event_consumed) {
				delete m_tmp_cmd_ptr;
			}
			m_tmp_cmd_ptr = 0;
	}
	else {
	}
	return;
}

void CStreamCtrlParserSM::onXmlCharacters(const std::string& text)
{
	// std::cerr << "on_characters(): " << text << std::endl;
	switch(m_xml_parser_state.open_state) {
	case XML_EV_ERROR:
		m_errorDescription += text;
		break;

	default:
		break;
	}
}

void CStreamCtrlParserSM::onXmlComment(const std::string& text)
{
	//std::cerr << "on_comment(): " << text << std::endl;
}

void CStreamCtrlParserSM::onXmlWarning(const std::string& text)
{
	//std::cerr << "on_warning(): " << text << std::endl;
}

void CStreamCtrlParserSM::onXmlError(const std::string& text)
{
	std::cerr << "on_error(): " << text << std::endl;
}




void CStreamCtrlParserSM::parseCmdID(const char** attrs, CmdStreamBase* cmd) {
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("cmdID") == 0) {
			cmd->m_cmd_id = CUtils::str2uint32(value);
		}
	}
}

void CStreamCtrlParserSM::parseEvClientStateArgs(     const char** attrs, evClientState* cmd) {
	string name_str;
	string addr_str;
	string vol_str;

	bool name_found = false;
	bool addr_found = false;
	bool vol_found = false;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("member_of_session") == 0) {
			name_str = value;
			name_found = true;
		}
		if(name.compare("session_srv") == 0) {
			addr_str = value;
			addr_found = true;
		}
		if(name.compare("volume") == 0) {
			vol_str = value;
			vol_found = true;
		}
	}

	if( !name_found || !addr_found || !vol_found ) {
		ostringstream oss;
		oss << "evClientState: ";
		if( !name_found ) {
			oss << " argument 'member_of_session' missing; ";
		}
		if( !addr_found ) {
			oss << " argument 'session_srv' missing; ";
		}
		if( !vol_found ) {
			oss << " argument 'volume' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_member_of_session = name_str;
	cmd->m_session_srv = str2ip_addr(addr_str);
	cmd->m_current_volume = CUtils::str2uint32(vol_str);
}

void CStreamCtrlParserSM::parseEvRequestJoinArgs(     const char** attrs, evRequestJoin* cmd) {
	string name_str;
	string addr_str;
	string ts_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("session_name") == 0) {
			name_str = value;
		}
		if(name.compare("mcast_addr") == 0) {
			addr_str = value;
		}
		if(name.compare("timesrv_port") == 0) {
			ts_str = value;
		}
	}

	if( name_str.empty() || addr_str.empty() || ts_str.empty() ) {
		ostringstream oss;
		oss << "evRequestJoin: ";
		if( name_str.empty() ) {
			oss << " argument 'session_name' missing; ";
		}
		if( addr_str.empty() ) {
			oss << " argument 'mcast_addr' missing; ";
		}
		if( ts_str.empty() ) {
			oss << " argument 'timesrv_port' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_session_name = name_str;
	cmd->m_mcast_addr = str2ip_addr(addr_str);
	cmd->m_timesrv_port = CUtils::str2uint32(ts_str);
}


void CStreamCtrlParserSM::parseEvRequestLeaveArgs( const char** attrs, evRequestLeave* cmd) {
	string name_str;
	string triggered_by_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("session_name") == 0) {
			name_str = value;
		}
		if(name.compare("triggered_by") == 0) {
			triggered_by_str = value;
		}
	}

	if( name_str.empty() || triggered_by_str.empty() ) {
		ostringstream oss;
		oss << "evRequestLeave: ";
		if( name_str.empty() ) {
			oss << " argument 'session_name' missing; ";
		}
		if( triggered_by_str.empty() ) {
			oss << " argument 'triggered_by' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_session_name = name_str;
	cmd->m_triggered_by_name = triggered_by_str;
}

void CStreamCtrlParserSM::parseEvJoinAcceptedArgs(    const char** attrs, evJoinAccepted* cmd) {
	string name_str;

	parseCmdID(attrs, cmd);
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		// argument may be empty
		if(name.compare("former_session") == 0) {
			name_str = value;
		}
	}

	cmd->m_former_session = name_str;
}

void CStreamCtrlParserSM::parseEvJoinRejectedArgs(    const char** attrs, evJoinRejected* cmd) {
	string name_str;
	string message_str;

	parseCmdID(attrs, cmd);
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("owner_session") == 0) {
			name_str = value;
		}
		if(name.compare("message") == 0) {
			message_str = value;
		}
	}

	if( name_str.empty() || message_str.empty() ) {
		ostringstream oss;
		oss << "evJoinRejected: ";
		if( name_str.empty() ) {
			oss << " argument 'owner_session' missing; ";
		}
		if( message_str.empty() ) {
			oss << " argument 'message' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_owner_session = name_str;
	cmd->m_message = message_str;
}

void CStreamCtrlParserSM::parseEvSessionErrorArgs(    const char** attrs, evSessionError* cmd) {
	string client_name_str;
	string member_of_session_str;
	string msg_str;
	string clock_offset_str;

	bool client_name_found = false;
	bool member_of_session_found = false;
	bool msg_found = false;
	bool clock_offset_found = false;

	parseCmdID(attrs, cmd);
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("client_name") == 0) {
			client_name_str = value;
			client_name_found = true;
		}
		if(name.compare("member_of_session") == 0) {
			member_of_session_str = value;
			member_of_session_found = true;
		}
		if(name.compare("msg") == 0) {
			msg_str = value;
			msg_found = true;
		}
		if(name.compare("clock_offset") == 0) {
			clock_offset_str = value;
			clock_offset_found = true;
		}
	}

	if( !client_name_found || !member_of_session_found || !msg_found || !clock_offset_found ) {
		ostringstream oss;
		oss << "evSessionError: ";
		if( !client_name_found ) {
			oss << " argument 'client_name' missing; ";
		}
		if( !member_of_session_found ) {
			oss << " argument 'member_of_session' missing; ";
		}
		if( !msg_found ) {
			oss << " argument 'msg' missing; ";
		}
		if( !clock_offset_found ) {
			oss << " argument 'clock_offset' missing; ";
		}
		throw ExRpcError(oss.str());
	}

	cmd->m_client_name = client_name_str;
	cmd->m_member_of_session = member_of_session_str;
	cmd->m_error_msg = msg_str;
	cmd->m_clock_offset = CDuration(CUtils::str2int64(clock_offset_str));
}

void CStreamCtrlParserSM::parseEvLeaveArgs(           const char** attrs, evLeave* cmd) {
	string name_str;
	string triggered_by_str;
	string addr_str;
	string vol_str;

	bool name_found = false;
	bool triggered_by_found = false;
	bool addr_found = false;
	bool vol_found = false;

	parseCmdID(attrs, cmd);
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("triggered_by") == 0) {
			triggered_by_str = value;
			triggered_by_found = true;
		}
		if(name.compare("member_of_session") == 0) {
			name_str = value;
			name_found = true;
		}
		if(name.compare("session_srv") == 0) {
			addr_str = value;
			addr_found = true;
		}
		if(name.compare("volume") == 0) {
			vol_str = value;
			vol_found = true;
		}
	}

	if( !triggered_by_found || !name_found || !addr_found || !vol_found ) {
		ostringstream oss;
		oss << "evClientState: ";
		if( !triggered_by_found ) {
			oss << " argument 'triggered_by' missing; ";
		}
		if( !name_found ) {
			oss << " argument 'member_of_session' missing; ";
		}
		if( !addr_found ) {
			oss << " argument 'session_srv' missing; ";
		}
		if( !vol_found ) {
			oss << " argument 'volume' missing; ";
		}
		throw ExRpcError(oss.str());
	}


	cmd->m_triggered_by_session = triggered_by_str;
	cmd->m_member_of_session = name_str;
	cmd->m_session_srv = str2ip_addr(addr_str);
	cmd->m_current_volume = CUtils::str2uint32(vol_str);

}

void CStreamCtrlParserSM::parseEvGetSessionStateArgs( const char** attrs, evGetSessionState* cmd) {
	string name_str;

	parseCmdID(attrs, cmd);
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		// argument may be empty
		if(name.compare("session_name") == 0) {
			name_str = value;
		}
	}
	if( name_str.empty() ) {
		ostringstream oss;
		oss << "evGetSessionState: ";
		if( name_str.empty() ) {
			oss << " argument 'session_name' missing; ";
		}
		throw ExRpcError(oss.str());
	}

	cmd->m_session_name = name_str;
}

void CStreamCtrlParserSM::parseEvSessionStateArgs( const char** attrs, evSessionState* cmd) {
	string name_str;
	string addr_str;
	string timesrv_port_str;
	string rtp_port_str;
	string vol_str;
	string media_provider_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("session_name") == 0) {
			name_str = value;
		}
		if(name.compare("timesrv_port") == 0) {
			timesrv_port_str = value;
		}
		if(name.compare("mcast_addr") == 0) {
			addr_str = value;
		}
		if(name.compare("rtp_unicast_port") == 0) {
			rtp_port_str = value;
		}
		if(name.compare("volume") == 0) {
				vol_str = value;
		}
		if(name.compare("media_provider") == 0) {
				media_provider_str = value;
		}
	}

	if( name_str.empty() || addr_str.empty() || timesrv_port_str.empty() || rtp_port_str.empty() || vol_str.empty() ) {
		ostringstream oss;
		oss << "evSessionState: ";
		if( name_str.empty() ) {
			oss << " argument 'session_name' missing; ";
		}
		if( addr_str.empty() ) {
			oss << " argument 'mcast_addr' missing; ";
		}
		if( timesrv_port_str.empty() ) {
			oss << " argument 'timesrv_port' missing; ";
		}
		if( rtp_port_str.empty() ) {
			oss << " argument 'rtp_unicast_port' missing; ";
		}
		if( vol_str.empty() ) {
			oss << " argument 'volume' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_session_name = name_str;
	cmd->m_mcast_addr = str2ip_addr(addr_str);
	cmd->m_timesrv_port = CUtils::str2uint32(timesrv_port_str);
	cmd->m_rtp_unicast_port = CUtils::str2uint32(rtp_port_str);
	cmd->m_volume = CUtils::str2uint32(vol_str);
	cmd->m_media_provider = media_provider_str;  // may be empty
}

void CStreamCtrlParserSM::parseEvResetStreamArgs(     const char** attrs, evResetStream* cmd) {
	string ssrc_str;;
	string rtp_ts_str;
	string pts_str;
	bool utc_pts_found = false;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc_str = value;
		}
		if(name.compare("rtp_ts") == 0) {
			rtp_ts_str = value;
		}
		if(name.compare("media_clock_pts") == 0) {
			pts_str = value;
		}
		if(name.compare("utc_media_clock_pts") == 0) {
			cmd->m_utc_media_clock_pts = value;
			utc_pts_found = true;
		}
	}

	if( ssrc_str.empty() || rtp_ts_str.empty() || pts_str.empty() ) {
		ostringstream oss;
		oss << "evResetStream: ";
		if( ssrc_str.empty() ) {
			oss << " argument 'ssrc' missing; ";
		}
		if( rtp_ts_str.empty() ) {
			oss << " argument 'rtp_ts' missing; ";
		}
		if( pts_str.empty() ) {
			oss << " argument 'media_clock_pts' missing; ";
		}
		if( !utc_pts_found ) {
			oss << " argument 'utc_media_clock_pts' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_ssrc = CUtils::str2uint32(ssrc_str);
	cmd->m_rtp_ts = CUtils::str2uint32(rtp_ts_str);
	cmd->m_media_clock_pts = CUtils::str2uint64(pts_str);
}

void CStreamCtrlParserSM::parseEvSyncStreamArgs(      const char** attrs, evSyncStream* cmd) {
	string ssrc_str;
	string rtp_ts_str;
	string num_channels_str;
	string sample_rate_str;
	string sample_size_str;
	string pts_str;
	bool utc_pts_found = false;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc_str = value;
		}
		if(name.compare("rtp_ts") == 0) {
			rtp_ts_str = value;
		}

		if(name.compare("num_channels") == 0) {
			num_channels_str = value;
		}
		if(name.compare("sample_rate") == 0) {
			sample_rate_str = value;
		}
		if(name.compare("sample_size") == 0) {
			sample_size_str = value;
		}

		if(name.compare("media_clock_pts") == 0) {
			pts_str = value;
		}
		if(name.compare("utc_media_clock_pts") == 0) {
			cmd->m_utc_media_clock_pts = value;
			utc_pts_found = true;
		}
	}

	if( ssrc_str.empty() || rtp_ts_str.empty() || num_channels_str.empty() || sample_rate_str.empty() || sample_size_str.empty() || pts_str.empty() ) {
		ostringstream oss;
		oss << "evResetStream: ";
		if( ssrc_str.empty() ) {
			oss << " argument 'ssrc' missing; ";
		}
		if( rtp_ts_str.empty() ) {
			oss << " argument 'rtp_ts' missing; ";
		}
		if( num_channels_str.empty() ) {
			oss << " argument 'num_channels' missing; ";
		}
		if( sample_rate_str.empty() ) {
			oss << " argument 'sample_rate' missing; ";
		}
		if( sample_size_str.empty() ) {
			oss << " argument 'sample_size' missing; ";
		}
		if( pts_str.empty() ) {
			oss << " argument 'media_clock_pts' missing; ";
		}
		if( !utc_pts_found ) {
			oss << " argument 'utc_media_clock_pts' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_ssrc = CUtils::str2uint32(ssrc_str);
	cmd->m_rtp_ts = CUtils::str2uint32(rtp_ts_str);
	cmd->m_num_channels = CUtils::str2uint32(num_channels_str);
	cmd->m_sample_rate = CUtils::str2uint32(sample_rate_str);
	cmd->m_sample_size = CUtils::str2uint32(sample_size_str);
	cmd->m_media_clock_pts = CUtils::str2uint64(pts_str);
	cmd->deserialize();
}

void CStreamCtrlParserSM::parseEvSetVolumeArgs(       const char** attrs, evSetVolume* cmd) {
	string ssrc_str;;
	string vol_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc_str = value;
		}
		if(name.compare("volume") == 0) {
			vol_str = value;
		}
	}

	if( ssrc_str.empty() || vol_str.empty() ) {
		ostringstream oss;
		oss << "evSetVolume: ";
		if( ssrc_str.empty() ) {
			oss << " argument 'ssrc' missing; ";
		}
		if( vol_str.empty() ) {
			oss << " argument 'volume' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_ssrc = CUtils::str2uint32(ssrc_str);
	cmd->m_volume = CUtils::str2uint32(vol_str);
}

void CStreamCtrlParserSM::parseEvVolumeArgs(       const char** attrs, evVolume* cmd) {
	string ssrc_str;;
	string vol_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc_str = value;
		}
		if(name.compare("volume") == 0) {
			vol_str = value;
		}
	}

	if( ssrc_str.empty() || vol_str.empty() ) {
		ostringstream oss;
		oss << "evVolume: ";
		if( ssrc_str.empty() ) {
			oss << " argument 'ssrc' missing; ";
		}
		if( vol_str.empty() ) {
			oss << " argument 'volume' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_ssrc = CUtils::str2uint32(ssrc_str);
	cmd->m_volume = CUtils::str2uint32(vol_str);
}

void CStreamCtrlParserSM::parseEvRequestProvArgs(const char** attrs, evRequestProv* cmd) {
	string input_ch_str;
	string descr_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("input_ch") == 0) {
			input_ch_str = value;
		}
		if(name.compare("description") == 0) {
			descr_str = value;
		}
	}

	if( input_ch_str.empty() || descr_str.empty() ) {
		ostringstream oss;
		oss << "evVolume: ";
		if( input_ch_str.empty() ) {
			oss << " argument 'input_ch' missing; ";
		}
		if( descr_str.empty() ) {
			oss << " argument 'description' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_input_ch = input_ch_str;
	cmd->m_description = descr_str;
}

void CStreamCtrlParserSM::parseEvProvAckArgs(const char** attrs, evProvAck* cmd) {
	string descr_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("description") == 0) {
			descr_str = value;
		}
	}

	if( descr_str.empty() ) {
		ostringstream oss;
		oss << "evProvAck: ";
		if( descr_str.empty() ) {
			oss << " argument 'description' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_description = descr_str;
}

void CStreamCtrlParserSM::parseEvProvRejArgs(const char** attrs, evProvRej* cmd) {
	string reason_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("reason") == 0) {
			reason_str = value;
		}
	}

	if( reason_str.empty() ) {
		ostringstream oss;
		oss << "evProvRejArgs: ";
		if( reason_str.empty() ) {
			oss << " argument 'reason' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_reason = reason_str;
}

void CStreamCtrlParserSM::parseEvRevokeProvArgs(const char** attrs, evRevokeProv* cmd) {
	string reason_str;

	parseCmdID(attrs, cmd);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("reason") == 0) {
			reason_str = value;
		}
	}

	if( reason_str.empty() ) {
		ostringstream oss;
		oss << "evRevokeProvArgs: ";
		if( reason_str.empty() ) {
			oss << " argument 'reason' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_reason = reason_str;
}


void CStreamCtrlParserSM::parseEvErrorArgs(           const char** attrs, evError* cmd) {
	string message_str;

	parseCmdID(attrs, cmd);
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("message") == 0) {
			message_str = value;
		}
	}

	if( message_str.empty() ) {
		ostringstream oss;
		oss << "evError: ";
		if( message_str.empty() ) {
			oss << " argument 'message' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	cmd->m_error_msg = message_str;
}


boost::asio::ip::address CStreamCtrlParserSM::str2ip_addr(std::string addr_str)
{
	boost::system::error_code ec;
	ip::address ipaddr = ip::address::from_string(addr_str, ec);

//	if(ec.value() == boost::system::errc::success) {
//		onJoinMCastGrp(m_tmp_cmdID, ipaddr);
//	}
//	else {
//		ostringstream oss;
//		oss << "joinMCastGroup: did not understand argument 'addr' (" << m_tmp_addr << "):" << endl;
//		oss << ec.message();
//		throw ExRpcError(oss.str());
//	}
	return ipaddr;
}



}  // namespace muroa
