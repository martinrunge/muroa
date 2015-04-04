 /**************************************************************************
 *                                                                         *
 *   IStreamCtrlRPC.h                                                      *
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


#ifndef ISTREAMCTRLRPC_H_
#define ISTREAMCTRLRPC_H_

#include <string>
#include <vector>
#include <MuroaExceptions.h>
#include <boost/asio.hpp>

class IStreamCtrl {
public:
	typedef enum state {   ROOT_STATE,
					 INFO_STATE,
		             SESSION_STATE
	} state_t;
};

class IStreamCtrlRPC : public IStreamCtrl {
public:
	virtual ~IStreamCtrlRPC() {}

	virtual void open() = 0;
	virtual void close() = 0;

	virtual void ack(uint32_t cmdID) = 0;
	virtual void error(uint32_t cmdID, int errorCode, std::string errmsg) = 0;

	virtual void joinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) = 0;
	virtual void joinSessionLeave() = 0;

	virtual void setTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port) = 0;
	virtual void getTimeSrv(uint32_t cmdID) = 0;

	virtual void getRTPPort(uint32_t cmdID) = 0;
	virtual void setRTPPort(uint32_t cmdID, uint32_t port) = 0;

	virtual void joinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) = 0;
	virtual void leaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) = 0;
	virtual void getMCastGrp(uint32_t cmdID) = 0;

	virtual void setStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts) = 0;
	virtual void getStreamTimeBase(uint32_t cmdID, uint32_t ssrc) = 0;

	virtual void resetStream(uint32_t cmdID, uint32_t ssrc) = 0;

	virtual void getVolume(uint32_t cmdID) = 0;
	virtual void setVolume(uint32_t cmdID, int percent) = 0;

	virtual void takeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) = 0;
	// newData:
	// when data is received from other end, feed it in here. The on... methods
	// will be called if a complete command was contained in the data.
	virtual void newData(const char* data, int len) = 0;

};

class IStreamCtrlCBs : public IStreamCtrl {
public:
	virtual ~IStreamCtrlCBs() {}

	virtual void onOpen(uint32_t cmdID) = 0;
	virtual void onClose(uint32_t cmdID) = 0;

	virtual void onAck(uint32_t cmdID) = 0;
	virtual void onError(uint32_t cmdID, int errorCode, std::string errmsg) = 0;

	virtual void onJoinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) = 0;
	virtual void onJoinSessionLeave() = 0;

	virtual void onTakeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv) = 0;

	virtual void onSetTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port) = 0;
	virtual void onGetTimeSrv(uint32_t cmdID) = 0;

	virtual void onGetRTPPort(uint32_t cmdID) = 0;
	virtual void onSetRTPPort(uint32_t cmdID, uint32_t port) = 0;

	virtual void onJoinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) = 0;
	virtual void onLeaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) = 0;
	virtual void onGetMCastGrp(uint32_t cmdID) = 0;

	virtual void onSetStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts) = 0;
	virtual void onGetStreamTimeBase(uint32_t cmdID, uint32_t ssrc) = 0;

	virtual void onResetStream(uint32_t cmdID, uint32_t ssrc) = 0;

	virtual void onGetVolume(uint32_t cmdID) = 0;
	virtual void onSetVolume(uint32_t cmdID, int percent) = 0;

	// onDataToSend:
	// called when there are serialized command ready to be sent to receiver
	virtual void onDataToSend(const char* data, int len) = 0;
};

#endif /* ISTREAMCTRLRPC_H_ */
