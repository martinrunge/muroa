 /**************************************************************************
 *                                                                         *
 *   CStreamCtrlXml.h                                                      *
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

#ifndef CMUROAXML_H_
#define CMUROAXML_H_

#include "cmds/CmdStreamBase.h"

#include "IStreamCtrlRPC.h"
#include "CStreamCtrlParserSM.h"
#include <MuroaExceptions.h>

#include <expat.h>
#include <cstdint>

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <boost/asio.hpp>

namespace muroa {

class CStreamCtrlXml: public muroa::IStreamCtrlRPC, public muroa::CStreamCtrlParserSM {
	enum state_t {   ROOT_STATE,
					 CMDINFO_STATE,
		             SESSION_STATE
	};

public:
	CStreamCtrlXml() throw(ExRpcError);
	~CStreamCtrlXml();

	void setup();
	void shutdown();

	void sendEvent(CmdStreamBase* ev);
//	void onRecvEvent(CmdStreamBase* ev);

	void newData(const char* data, int len);

private:

	void sendEvClientState     ( CmdStreamBase* ev);
	void sendEvRequestJoin     ( CmdStreamBase* ev);
	void sendEvJoinAccepted    ( CmdStreamBase* ev);
	void sendEvJoinRejected    ( CmdStreamBase* ev);
	void sendEvLeave           ( CmdStreamBase* ev);
	void sendEvGetSessionState ( CmdStreamBase* ev);
	void sendEvSessionState    ( CmdStreamBase* ev);
	void sendEvResetStream     ( CmdStreamBase* ev);
	void sendEvSyncStream      ( CmdStreamBase* ev);
	void sendEvSetVolume       ( CmdStreamBase* ev);
	void sendEvAck             ( CmdStreamBase* ev);
	void sendEvError           ( CmdStreamBase* ev);

	void sendData(std::string data);

    static void XMLCALL startTagHandler(void *inst_ptr, const char *el, const char **attr);
    static void XMLCALL endTagHandler(void *inst_ptr, const char *el);
    static void XMLCALL characterHandler(void *inst_ptr, const char *s, int len);

    typedef void (CStreamCtrlXml::*serializeFunc_t)(CmdStreamBase*);
	std::unordered_map<std::type_index, serializeFunc_t> type_serializers;

    XML_Parser m_parser;
	uint32_t m_sessionID;
};

}

#endif /* CMUROAXML_H_ */
