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

#include "IStreamCtrlRPC.h"
#include "CStreamCtrlParserSM.h"
#include <MuroaExceptions.h>

#include <expat.h>
#include <cstdint>

#include <boost/asio.hpp>

namespace muroa {

class CStreamCtrlXml: public muroa::IStreamCtrlRPC, public muroa::CStreamCtrlParserSM {
	enum state_t {   ROOT_STATE,
					 INFO_STATE,
		             SESSION_STATE
	};

public:
	CStreamCtrlXml() throw(ExRpcError);
	~CStreamCtrlXml();

	void setup();
	void shutdown();

	void ack(uint32_t cmdID);
	void error(uint32_t cmdID, int errorCode, std::string errmsg);

	virtual void joinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv);
	void joinSessionLeave();

	virtual void takeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv);

	virtual void setTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port);
	virtual void getTimeSrv(uint32_t cmdID);

	virtual void getRTPPort(uint32_t cmdID);
	virtual void setRTPPort(uint32_t cmdID, uint32_t port);

	virtual void joinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr);
	virtual void leaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr);
	virtual void getMCastGrp(uint32_t cmdID);

	virtual void setStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts);
	virtual void getStreamTimeBase(uint32_t cmdID, uint32_t ssrc);

	virtual void resetStream(uint32_t cmdID, uint32_t ssrc);

	virtual void getVolume(uint32_t cmdID);
	virtual void setVolume(uint32_t cmdID, int percent);

	void newData(const char* data, int len);

private:
	void sendData(std::string data);

    static void XMLCALL startTagHandler(void *inst_ptr, const char *el, const char **attr);
    static void XMLCALL endTagHandler(void *inst_ptr, const char *el);
    static void XMLCALL characterHandler(void *inst_ptr, const char *s, int len);


	XML_Parser m_parser;

	uint32_t m_sessionID;
};

}

#endif /* CMUROAXML_H_ */
