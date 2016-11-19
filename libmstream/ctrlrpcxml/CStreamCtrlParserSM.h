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

#include "cmds/CmdStream.h"

namespace muroa {

class CStreamCtrlParserSM : public IStreamCtrlCBs {
public:
	enum root_state_t {   XML_ROOT_STATE,
						  XML_OPEN_STATE
	};


	enum open_state_t {XML_OPEN_STATE_ROOT,
					      XML_EV_CLIENT_STATE,
						  XML_EV_REQUEST_JOIN,
						  XML_EV_REQUEST_LEAVE,
		                  XML_EV_JOIN_ACCEPTED,
						  XML_EV_JOIN_REJECTED,
						  XML_EV_LEAVE,
						  XML_EV_GET_SESSION_STATE,
						  XML_EV_SESSION_STATE,
						  XML_EV_RESET_STREAM,
						  XML_EV_SYNC_STREAM,
						  XML_EV_SET_VOLUME,
						  XML_EV_ERROR
	};

	typedef struct {
		root_state_t root_state;
		open_state_t open_state;
	} parser_state_t;


	CStreamCtrlParserSM();
	virtual ~CStreamCtrlParserSM() = 0;

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
	virtual bool onEvent(CmdStreamBase* ev) = 0;


private:
	std::string m_concatenate_dummy;
	std::string  m_errorDescription;

	enum action_flag { INIT, START, END};

	parser_state_t m_xml_parser_state;

	int m_xml_tag_depth;
	int m_tag_unknown_depth;
	bool m_in_unknown_tag;

	bool m_concatenate_chars_state;

	uint32_t m_tmp_cmdID;
	int m_errorCode;
	CmdStreamBase* m_tmp_cmd_ptr;



	void parseCmdID(const char** attrs, CmdStreamBase* cmd );
	boost::asio::ip::address str2ip_addr(std::string addr_str);

	void parseEvClientStateArgs(     const char** attrs, evClientState* cmd);
	void parseEvRequestJoinArgs(     const char** attrs, evRequestJoin* cmd);
	void parseEvRequestLeaveArgs(    const char** attrs, evRequestLeave* cmd);
	void parseEvJoinAcceptedArgs(    const char** attrs, evJoinAccepted* cmd);
	void parseEvJoinRejectedArgs(    const char** attrs, evJoinRejected* cmd);
	void parseEvLeaveArgs(           const char** attrs, evLeave* cmd);
	void parseEvGetSessionStateArgs( const char** attrs, evGetSessionState* cmd);
	void parseEvSessionStateArgs(    const char** attrs, evSessionState* cmd);
	void parseEvResetStreamArgs(     const char** attrs, evResetStream* cmd);
	void parseEvSyncStreamArgs(      const char** attrs, evSyncStream* cmd);
	void parseEvSetVolumeArgs(       const char** attrs, evSetVolume* cmd);
	void parseEvErrorArgs(           const char** attrs, evError* cmd);

};
}
#endif
