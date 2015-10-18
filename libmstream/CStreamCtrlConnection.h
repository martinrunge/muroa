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
// #include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>



#include <ISrvSMActions.h>
#include <CStreamSrvSM.h>



namespace bip=boost::asio::ip;

namespace muroa {

class CStreamConnection;
class CStreamServer;

class CStreamCtrlConnection : public bip::tcp::socket, public muroa::CStreamCtrlXml, public muroa::ISrvSMActions {
public:
	CStreamCtrlConnection(std::string serviceName, muroa::CStreamServer* stream_server, boost::asio::io_service& io_service);
	virtual ~CStreamCtrlConnection();

	void connect(const endpoint_type & peer_endpoint, boost::system::error_code & ec);

	void openStreamConnection();
	void closeStreamConnection();

	void onSetup( );
	void onShutdown( );
	bool onEvent(muroa::CmdStreamBase* ev);

	void dataReceived( boost::array<char, 8192> buffer, int length);

	const bool m_useMulticastRTP() const { return m_use_multicast_rtp; };

	const std::string& getServiceName() const {
		return m_serviceName;
	}

	void setServiceName(const std::string& serviceName) {
		m_serviceName = serviceName;
	}

	CStreamConnection*& getStreamConnection() {
		return m_stream_connection;
	}
//	void setStreamConnection(CStreamConnection* const streamConnection) {
//		m_stream_connection = streamConnection;
//	}

	CStreamServer* getStreamServer() {
		return m_stream_server;
	}

	void setStreamServer(CStreamServer* const streamServer) {
		m_stream_server = streamServer;
	}

	void reportError(std::string);
    void reportError(const evJoinRejected* evt);

	void reportTimeout(std::string);
	void reportClientState(const CmdStreamBase* evt);
	void requestJoin(const evRequestJoin* evt);
	void gotSessionState(const CmdStreamBase* cmd);

	void sendAck(const evJoinAccepted* evt);

private:
	void start_read();
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	void onDataToSend(const char* data, int len);

	CStreamServer* m_stream_server;
	std::string m_serviceName;

	CStreamConnection* m_stream_connection;

	muroa::CStreamSrvSM m_srv_sm;

	unsigned short m_RTP_port;
	bool m_use_multicast_rtp;
	/// Buffer for incoming data.
	boost::array<char, 8192> m_buffer;

};

} /* namespace muroa */

#endif /* CCTRLCONNECTION_H_ */
