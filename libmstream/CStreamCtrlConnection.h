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

#ifndef CCTRLCONNECTION_H_
#define CCTRLCONNECTION_H_

#include "ctrlrpcxml/CStreamCtrlXml.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class CStreamConnection;
class CStreamServer;

namespace bip=boost::asio::ip;

namespace muroa {

class CStreamCtrlConnection : public bip::tcp::socket, public muroa::CStreamCtrlXml {
public:
	CStreamCtrlConnection(std::string serviceName, CStreamServer* stream_server, boost::asio::io_service& io_service);
	virtual ~CStreamCtrlConnection();

	void openStreamConnection();
	void closeStreamConnection();

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

	const std::string& getServiceName() const {
		return m_serviceName;
	}

	void setServiceName(const std::string& serviceName) {
		m_serviceName = serviceName;
	}

	CStreamConnection*& getStreamConnection() {
		return m_stream_connection;
	}
	void setStreamConnection(CStreamConnection* const streamConnection) {
		m_stream_connection = streamConnection;
	}

	CStreamServer* getStreamServer() {
		return m_stream_server;
	}

	void setStreamServer(CStreamServer* const streamServer) {
		m_stream_server = streamServer;
	}


private:
	void onDataToSend(const char* data, int len);

	CStreamServer* m_stream_server;
	std::string m_serviceName;

	CStreamConnection* m_stream_connection;


	unsigned short m_RTP_port;
};

} /* namespace muroa */

#endif /* CCTRLCONNECTION_H_ */
