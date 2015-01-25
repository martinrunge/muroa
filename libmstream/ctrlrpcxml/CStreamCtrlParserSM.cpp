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
#include "ctrlrpcxml/StreamCtrlXMLCmds.h"

#include <iostream>
#include <assert.h>

using namespace std;
using namespace muroa;
using namespace boost::asio;

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
	m_xml_parser_state.info_state = XML_INFO_STATE_ROOT;
	m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
	m_xml_tag_depth = 0;
	m_tag_unknown_depth = 0;
	m_in_unknown_tag = false;
}

CStreamCtrlParserSM::~CStreamCtrlParserSM()
{
}

void CStreamCtrlParserSM::onXmlVersion() {

}

void CStreamCtrlParserSM::setState(IStreamCtrl::state_t state) {
	m_state = state;
}

IStreamCtrl::state_t CStreamCtrlParserSM::getState() {
	return m_state;
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
	// std::cerr << "start node name=" << name << std::endl;

	// Print attributes:
	//for(int i=0; attributes[i]; i+=2)
	//{
		// std::cerr << attributes[i] << " = " << attributes[i + 1] << std::endl;
	//}
	// error reporting is defined in any state
	if(name.compare(StreamCtrlXMLCmds::ack) == 0) {
		parseCmdID(attributes);
		onAck(m_tmp_cmdID);
	}
	if(name.compare(StreamCtrlXMLCmds::error) == 0) {
		parseErrorArgs(attributes);
	}
	else {
		switch (m_state) {
			case SESSION_STATE:
				// state machine is in state 'IN_SESSION_STATE'. Whatever can happen from here is handled by the function 'sessionState'
				sessionState(START, name, attributes);
				break;

			case INFO_STATE:
				// ... whatever can happen from here is handled by the function 'infoState'
				infoState(START, name, attributes);
				break;

			case ROOT_STATE:
			default:

				// the state machine is in state 'ROOT_STATE'. Only switch into INFO_SATE or SESSION_STATE is allowed from this state.
				if(name.compare(StreamCtrlXMLCmds::joinSession) == 0) {
					sessionState(INIT, name, attributes);
				}
				if(name.compare(StreamCtrlXMLCmds::requestInfo) == 0) {
					infoState(INIT, name, attributes);
				}
				break;
		}
	}
	return;
}

void CStreamCtrlParserSM::onXmlEndElement(const std::string& name)
{
	// std::cerr << "on_end_element()" << std::endl;
	const char **null_ptr( 0 );

	// error reporting alowed in any state
	if(name.compare(StreamCtrlXMLCmds::error) == 0) {
		onError(m_tmp_cmdID, m_errorCode, m_errorMsg);
	}
	else {
		switch (m_state) {
			case SESSION_STATE:
				// state machine is in state 'IN_SESSION_STATE'. Whatever can happen from here is handled by the function 'sessionState'
				sessionState(END, name, null_ptr);
				break;

			case INFO_STATE:
				// ... whatever can happen from here is handled by the function 'infoState'
				infoState(END, name, null_ptr);
				break;

			case ROOT_STATE:
			default:

				// state machine is in state 'ROOT_STATE'. Whatever can happen from here is handled by the function 'rootState'
				if(name.compare(StreamCtrlXMLCmds::joinSession) == 0) {
					m_xml_parser_state.root_state = XML_ROOT_STATE;
				}
				if(name.compare(StreamCtrlXMLCmds::requestInfo) == 0) {
					m_xml_parser_state.root_state = XML_ROOT_STATE;
				}
				break;
		}
	}
	return;
}

void CStreamCtrlParserSM::onXmlCharacters(const std::string& text)
{
	// std::cerr << "on_characters(): " << text << std::endl;
	switch(m_xml_parser_state.session_state) {
	case XML_ERROR:
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

void CStreamCtrlParserSM::infoState(const action_flag& init_start_end, const std::string& name, const char** attrs) {
	switch (init_start_end) {
	case INIT:
		m_xml_tag_depth = 1;
		parseInfoArgs(attrs);
		m_xml_parser_state.root_state = XML_INFO_STATE;
		break;

	case START:
		if (name.compare(StreamCtrlXMLCmds::getState) == 0) {
			m_xml_parser_state.info_state = XML_INFO_GET_STATE;
		}
		else if (name.compare(StreamCtrlXMLCmds::takeFromSession) == 0) {
			parseTakeFromSessionArgs(attrs);
		}

		else {
			cerr << "CParserStateMachine::processSessionState (START): unknown tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth++;
			m_xml_tag_depth++;
		}
		break;
	case END:
		if (name.compare(StreamCtrlXMLCmds::getState) == 0) {
			m_xml_parser_state.info_state = XML_INFO_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::takeFromSession) == 0) {
		}
		if (name.compare(StreamCtrlXMLCmds::requestInfo) == 0) {
			m_xml_parser_state.root_state = XML_ROOT_STATE;
		}
		else {
			cerr << "unknown end tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth--;
			m_xml_tag_depth--;
		}
		break;
	}
}

void CStreamCtrlParserSM::parseInfoArgs(const char** attrs) {

}


void CStreamCtrlParserSM::sessionState(const action_flag& init_start_end, const std::string& name, const char** attrs) {

	// std::cerr << "CParserStateMachine::sessionState" << std::endl;
	switch (init_start_end) {
	case INIT:
		m_xml_tag_depth = 1;
		parseJoinSessionArgs(attrs);
		m_xml_parser_state.root_state = XML_SESSION_STATE;
		onJoinSession(m_tmp_cmdID, m_tmp_name, m_tmp_addr);
		break;

	case START:
		if (name.compare(StreamCtrlXMLCmds::getTimeSrv) == 0) {
			parseGetTimeSrvArgs(attrs);
			m_xml_parser_state.session_state = XML_GET_TIME_SRV;
		}
		else if (name.compare(StreamCtrlXMLCmds::setTimeSrv) == 0) {
			parseSetTimeSrvArgs(attrs);
			m_xml_parser_state.session_state = XML_SET_TIME_SRV;
		}
		else if (name.compare(StreamCtrlXMLCmds::getRTPPort) == 0) {
			parseGetRtpPortArgs(attrs);
			m_xml_parser_state.session_state = XML_GET_RTP_PORT;
		}
		else if (name.compare(StreamCtrlXMLCmds::setRTPPort) == 0) {
			parseSetRtpPortArgs(attrs);
			m_xml_parser_state.session_state = XML_SET_RTP_PORT;
		}
		else if (name.compare(StreamCtrlXMLCmds::joinMCastGrp) == 0) {
			parseJoinMCastGrpArgs(attrs);
			m_xml_parser_state.session_state = XML_JOIN_MCAST_GRP;
		}
		else if (name.compare(StreamCtrlXMLCmds::leaveMCastGrp) == 0) {
			parseLeaveMCastGrpArgs(attrs);
			m_xml_parser_state.session_state = XML_LEAVE_MCAST_GRP;
		}
		else if (name.compare(StreamCtrlXMLCmds::getMCastGrp) == 0) {
			parseGetMCastGrpArgs(attrs);
			m_xml_parser_state.session_state = XML_GET_MCAST_GRP;
		}
		else if (name.compare(StreamCtrlXMLCmds::getStreamTimeBase) == 0) {
			parseGetStreamTimebaseArgs(attrs);
			m_xml_parser_state.session_state = XML_GET_TIME_BASE;
		}
		else if (name.compare(StreamCtrlXMLCmds::setStreamTimeBase) == 0) {
			parseSetStreamTimebaseArgs(attrs);
			m_xml_parser_state.session_state = XML_SET_TIME_BASE;
		}
		else if (name.compare(StreamCtrlXMLCmds::resetStream) == 0) {
			parseResetStreamArgs(attrs);
			m_xml_parser_state.session_state = XML_RESET_STREAM;
		}
		else if (name.compare(StreamCtrlXMLCmds::getVolume) == 0) {
			parseGetVolumeArgs(attrs);
			m_xml_parser_state.session_state = XML_GET_VOLUME;
		}
		else if (name.compare(StreamCtrlXMLCmds::setVolume) == 0) {
			parseSetVolumeArgs(attrs);
			m_xml_parser_state.session_state = XML_SET_VOLUME;
		}

		else {
			cerr << "CParserStateMachine::processSessionState (START): unknown tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth++;
			m_xml_tag_depth++;
		}
		break;

	case END:
		if (name.compare(StreamCtrlXMLCmds::getTimeSrv) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::setTimeSrv) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::getRTPPort) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::setRTPPort) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::joinMCastGrp) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::leaveMCastGrp) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::getMCastGrp) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::getStreamTimeBase) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::setStreamTimeBase) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::resetStream) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::getVolume) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::setVolume) == 0) {
			m_xml_parser_state.session_state = XML_SESSION_STATE_ROOT;
		}
		else if (name.compare(StreamCtrlXMLCmds::joinSession) == 0) {
			m_xml_parser_state.root_state = XML_ROOT_STATE;
			onJoinSessionLeave( m_joinSession_Name );
		}
		else {
			cerr << "unknown end tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth--;
			m_xml_tag_depth--;
		}
		break;
	default:
		break;
	}

}

void CStreamCtrlParserSM::parseCmdID(const char** attrs) {
	m_tmp_cmdID = 0;

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("cmdID") == 0) {
			m_tmp_cmdID = CUtils::str2uint32(value);
		}
	}
}

boost::asio::ip::address CStreamCtrlParserSM::str2ip_addr(std::string addr_str)
{
	boost::system::error_code ec;
	ip::address ipaddr = ip::address::from_string(addr_str, ec);

	if(ec.value() == boost::system::errc::success) {
		onJoinMCastGrp(m_tmp_cmdID, ipaddr);
	}
	else {
		ostringstream oss;
		oss << "joinMCastGroup: did not understand argument 'addr' (" << m_tmp_addr << "):" << endl;
		oss << ec.message();
		throw ExRpcError(oss.str());
	}
	return ipaddr;
}


void CStreamCtrlParserSM::parseJoinSessionArgs(const char** attrs) {
	m_tmp_name.clear();
	string addr_str;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("name") == 0) {
			m_tmp_name = value;
		}
		if(name.compare("addr") == 0) {
			addr_str = value;
		}
	}

	if( m_tmp_name.empty() || addr_str.empty() ) {
		ostringstream oss;
		oss << "addToSession: ";
		if( m_tmp_name.empty() ) {
			oss << " argument 'name' missing; ";
		}
		if( addr_str.empty() ) {
			oss << " argument 'addr' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	m_joinSession_CmdID = m_tmp_cmdID;
	m_joinSession_Name = m_tmp_name;

	m_tmp_addr = str2ip_addr(addr_str);
}

void CStreamCtrlParserSM::parseTakeFromSessionArgs(const char** attrs) {
	m_tmp_name.clear();
	string addr_str;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("name") == 0) {
			m_tmp_name = value;
		}
		if(name.compare("addr") == 0) {
			addr_str = value;
		}
	}

	if( m_tmp_name.empty() || addr_str.empty() ) {
		ostringstream oss;
		oss << "addToSession: ";
		if( m_tmp_name.empty() ) {
			oss << " argument 'name' missing; ";
		}
		if( addr_str.empty() ) {
			oss << " argument 'addr' missing; ";
		}
		throw ExRpcError(oss.str());
	}

	m_tmp_addr = str2ip_addr(addr_str);

	onTakeFromSession(m_tmp_cmdID, m_tmp_name, m_tmp_addr);
}

void CStreamCtrlParserSM::parseGetTimeSrvArgs(const char** attrs) {
	parseCmdID(attrs);
	onGetTimeSrv(m_tmp_cmdID);
}

void CStreamCtrlParserSM::parseSetTimeSrvArgs(const char** attrs) {
	m_tmp_port = 0;
	string addr_str;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("addr") == 0) {
			addr_str = value;
		}
		if(name.compare("port") == 0) {
			m_tmp_port = CUtils::str2uint32(value);
		}
	}

	if( addr_str.empty() || m_tmp_port == 0 ) {
		ostringstream oss;
		oss << "setTimeSrv: ";
		if( addr_str.empty() ) {
			oss << " argument 'addr' missing; ";
		}
		if( m_tmp_port == 0 ) {
			oss << " argument 'port' missing; ";
		}
		throw ExRpcError(oss.str());
	}
	m_tmp_addr = str2ip_addr(addr_str);

	onSetTimeSrv(m_tmp_cmdID, m_tmp_addr, m_tmp_port);
}

void CStreamCtrlParserSM::parseGetRtpPortArgs(const char** attrs) {
	parseCmdID(attrs);
	onGetRTPPort(m_tmp_cmdID);
}

void CStreamCtrlParserSM::parseSetRtpPortArgs(const char** attrs) {
	m_tmp_port = 0;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("port") == 0) {
			m_tmp_port = CUtils::str2uint32(value);
		}
	}

	if( m_tmp_port == 0 ) {
		ostringstream oss;
		oss << "setRtpPort: ";
		oss << " argument 'port' missing; ";
		throw ExRpcError(oss.str());
	}

	onSetRTPPort(m_tmp_cmdID, m_tmp_port);
}

void CStreamCtrlParserSM::parseJoinMCastGrpArgs(const char** attrs) {
	string addr_str;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("addr") == 0) {
			addr_str = value;
		}
	}

	m_tmp_addr = str2ip_addr(addr_str);

	onJoinMCastGrp(m_tmp_cmdID, m_tmp_addr);
}

void CStreamCtrlParserSM::parseLeaveMCastGrpArgs(const char** attrs) {
	string addr_str;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("addr") == 0) {
			addr_str = value;
		}
	}

	m_tmp_addr = str2ip_addr(addr_str);

	onLeaveMCastGrp(m_tmp_cmdID, m_tmp_addr);
}

void CStreamCtrlParserSM::parseGetMCastGrpArgs(const char** attrs) {
	parseCmdID(attrs);
	onGetMCastGrp(m_tmp_cmdID);
}

void CStreamCtrlParserSM::parseSetStreamTimebaseArgs(const char** attrs) {
	uint32_t ssrc = 0;
	bool ssrc_given = false;

	uint64_t rtp_ts = 0;
	bool rtp_ts_given = false;

	uint64_t pts = 0;
	bool pts_given = false;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc = CUtils::str2uint32(value);
			ssrc_given = true;
		}
		if(name.compare("rtp_ts") == 0) {
			rtp_ts = CUtils::str2uint64(value);
			rtp_ts_given = true;
		}
		if(name.compare("pts") == 0) {
			pts = CUtils::str2uint64(value);
			pts_given = true;
		}
	}

	if( ssrc_given && rtp_ts_given && pts_given) {
		onSetStreamTimeBase(m_tmp_cmdID, ssrc, rtp_ts, pts);
	}
	else {
		ostringstream oss;
		oss << "SetStreamTimeBase: ";
		if( !ssrc_given ) {
			oss << " argument 'ssrc' missing; ";
		}
		if( !rtp_ts_given ) {
			oss << " argument 'rtp_ts' missing; ";
		}
		if( !pts_given ) {
			oss << " argument 'pts' missing; ";
		}
		throw ExRpcError(oss.str());
	}
}

void CStreamCtrlParserSM::parseGetStreamTimebaseArgs(const char** attrs) {
	uint32_t ssrc = 0;
	bool ssrc_given = false;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc = CUtils::str2uint32(value);
			ssrc_given = true;
		}
	}

	if( ssrc_given ) {
		onGetStreamTimeBase(m_tmp_cmdID, ssrc);
	}
	else {
		ostringstream oss;
		oss << "GetStreamTimeBase: ";
		oss << " argument 'ssrc' missing; ";
		throw ExRpcError(oss.str());
	}
}

void CStreamCtrlParserSM::parseResetStreamArgs(const char** attrs) {
	uint32_t ssrc = 0;
	bool ssrc_given = false;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("ssrc") == 0) {
			ssrc = CUtils::str2uint32(value);
			ssrc_given = true;
		}
	}

	if( ssrc_given ) {
		onResetStream(m_tmp_cmdID, ssrc);
	}
	else {
		ostringstream oss;
		oss << "resetStream: ";
		oss << " argument 'ssrc' missing; ";
		throw ExRpcError(oss.str());
	}
}

void CStreamCtrlParserSM::parseGetVolumeArgs(const char** attrs) {
	parseCmdID(attrs);
	onGetVolume(m_tmp_cmdID);
}

void CStreamCtrlParserSM::parseSetVolumeArgs(const char** attrs) {
	int percent = 0;
	bool percent_given = false;

	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("percent") == 0) {
			percent = CUtils::str2long(value);
			percent_given = true;
		}
	}

	if( percent_given ) {
		onSetVolume(m_tmp_cmdID, percent);
	}
	else {
		ostringstream oss;
		oss << "setVolume: ";
		oss << " argument 'percent' missing; ";
		throw ExRpcError(oss.str());
	}
}

void CStreamCtrlParserSM::parseErrorArgs(const char** attrs) {
	parseCmdID(attrs);

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

        if(name.compare("code") == 0) {
			m_errorCode = CUtils::str2long(value);
		}
		else if(name.compare("msg") == 0) {
			m_errorMsg = value;
		}
	}
}
