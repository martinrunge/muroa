 /**************************************************************************
 *                                                                         *
 *   CStreamCtrlParserSM.h                                                 *
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

#ifndef CPARSERSTATEMACHINE_H
#define CPARSERSTATEMACHINE_H

/**
state machine for the command parser

@author Martin Runge
 */

#include <string>
#include <vector>
#include "IStreamCtrlRPC.h"



class CStreamCtrlParserSM : public IStreamCtrlCBs {
public:
	enum root_state_t {   XML_ROOT_STATE,
						  XML_INFO_STATE,
		                  XML_SESSION_STATE
	};

	enum info_state_t {   XML_INFO_STATE_ROOT,
		                  XML_INFO_GET_STATE
	};

	enum session_state_t {XML_SESSION_STATE_ROOT,
					      XML_ADD_TO_SESSION,
						  XML_REMOVE_FROM_SESSION,
		                  XML_GET_TIME_SRV,
						  XML_SET_TIME_SRV,
						  XML_GET_RTP_PORT,
						  XML_SET_RTP_PORT,
						  XML_JOIN_MCAST_GRP,
						  XML_LEAVE_MCAST_GRP,
						  XML_GET_MCAST_GRP,
						  XML_SET_TIME_BASE,
						  XML_GET_TIME_BASE,
						  XML_RESET_STREAM,
						  XML_GET_VOLUME,
						  XML_SET_VOLUME,
						  XML_ERROR
	};

	typedef struct {
		root_state_t root_state;
		info_state_t info_state;
		session_state_t session_state;
	} parser_state_t;


	CStreamCtrlParserSM();
	virtual ~CStreamCtrlParserSM() = 0;

	void setState(IStreamCtrl::state_t state);
	IStreamCtrl::state_t getState();

	void onXmlStartDocument();
	void onXmlEndDocument();
	void onXmlStartElement(const std::string& name, const char** attributes);
	void onXmlEndElement(const std::string& name);
	void onXmlCharacters(const std::string& text);
	void onXmlComment(const std::string& text);
	void onXmlWarning(const std::string& text);
	void onXmlError(const std::string& text);

	void onXmlVersion();

protected:
    void reset();

private:
	std::string m_concatenate_dummy;

	enum action_flag { INIT, START, END};

	state_t m_state;
	parser_state_t m_xml_parser_state;

	int m_xml_tag_depth;
	int m_tag_unknown_depth;
	bool m_in_unknown_tag;

	bool m_concatenate_chars_state;

	uint32_t m_joinSession_CmdID;
	std::string m_joinSession_Name;

	uint32_t m_tmp_cmdID;
	int m_errorCode;
	std::string m_errorMsg;

	std::string m_errorDescription;
	std::string m_get_text;
	std::string m_edit_text;

	std::string m_tmp_name;
	boost::asio::ip::address m_tmp_addr;
	int m_tmp_port;

	void infoState(const action_flag& init_start_end, const std::string& name, const char** attrs);
	void sessionState(const action_flag& init_start_end, const std::string& name, const char** attrs);

	void parseCmdID(const char** attrs);
	boost::asio::ip::address str2ip_addr(std::string addr_str);

	void parseInfoArgs(const char** attrs);

	void parseJoinSessionArgs(const char** attrs);
	void parseTakeFromSessionArgs(const char** attrs);

	void parseGetTimeSrvArgs(const char** attrs);
	void parseSetTimeSrvArgs(const char** attrs);

	void parseGetRtpPortArgs(const char** attrs);
	void parseSetRtpPortArgs(const char** attrs);

	void parseJoinMCastGrpArgs(const char** attrs);
	void parseLeaveMCastGrpArgs(const char** attrs);
	void parseGetMCastGrpArgs(const char** attrs);

	void parseSetStreamTimebaseArgs(const char** attrs);
	void parseGetStreamTimebaseArgs(const char** attrs);

	void parseResetStreamArgs(const char** attrs);

	void parseGetVolumeArgs(const char** attrs);
	void parseSetVolumeArgs(const char** attrs);

	void parseErrorArgs(const char** attrs);

};

#endif
