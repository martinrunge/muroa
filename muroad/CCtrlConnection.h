/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCtrlConnection.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef MUROAD_CCTRLCONNECTION_H_
#define MUROAD_CCTRLCONNECTION_H_

#include <CTcpConnection.h>
#include <ctrlrpcxml/CStreamCtrlXml.h>

class CCtrlConnection: public muroa::CTcpConnection, muroa::CStreamCtrlXml {
public:
	static CCtrlConnection* create(boost::asio::io_service& io_service) {
	    return new CCtrlConnection(io_service);
	}
	virtual ~CCtrlConnection();

	void onOpen(uint32_t cmdID);
	void onClose(uint32_t cmdID);

	void onAck(uint32_t cmdID);
	void onError(uint32_t cmdID, int errorCode, std::string errmsg);

	void onJoinSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv);
	void onJoinSessionLeave();

	void onTakeFromSession(uint32_t cmdID, std::string name, boost::asio::ip::address session_srv);

	void onSetTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port);
	void onGetTimeSrv(uint32_t cmdID);

	void onGetRTPPort(uint32_t cmdID);
	void onSetRTPPort(uint32_t cmdID, uint32_t port);

	void onJoinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr);
	void onLeaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr);
	void onGetMCastGrp(uint32_t cmdID);

	void onSetStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts);
	void onGetStreamTimeBase(uint32_t cmdID, uint32_t ssrc);

	void onResetStream(uint32_t cmdID, uint32_t ssrc);

	void onGetVolume(uint32_t cmdID);
	void onSetVolume(uint32_t cmdID, int percent);


	void dataReceived( boost::array<char, 8192> buffer, int length);

private:
	CCtrlConnection(boost::asio::io_service& io_service);

	void onDataToSend(const char* data, int len);
};

#endif /* MUROAD_CCTRLCONNECTION_H_ */
