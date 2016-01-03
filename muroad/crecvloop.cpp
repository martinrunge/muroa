/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
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
#include <CMediaStreamConnection.h>
#include <CPacketRingBuffer.h>
#include "crecvloop.h"
#include "caudioframe.h"
#include "csocket.h"
#include "CApp.h"

#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>


using namespace std;
using namespace muroa;
using namespace log4cplus;
using namespace boost::asio::ip;


CRecvloop::CRecvloop(CMediaStreamConnection* parent, CPacketRingBuffer* packet_ringbuffer):
				CThreadSlave(), m_tmp_rtp_packet(0)
{

	m_media_stream_conn = parent;
	m_timing_logger = Logger::getInstance("timing");

	m_packet_ringbuffer = packet_ringbuffer;

	m_max_idle = CApp::settings().getConfigVal("muroad.MaxIdle", 10);

	unsigned short port = CApp::settings().getPersisentVal("muroad.RTPport", 0);
	if(port == 0) {
		port = CApp::settings().getConfigVal("muroad.RTPport", 44400);
	}

	m_socket = new CSocket(SOCK_DGRAM, port, true);
	CApp::settings().setPersistentVal("muroad.RTPport", m_socket->getPort());

	m_socket->recordSenderWithRecv(true);
	m_socket->setNonBlocking(500000);

}


CRecvloop::~CRecvloop()
{
	delete m_socket;
}

int CRecvloop::getRTPPort() {
	return m_socket->getPort();
}



void CRecvloop::DoLoop()
{
	if(m_tmp_rtp_packet == 0) {
		m_tmp_rtp_packet = new CRTPPacket();
	}
	int num = m_socket->read(m_tmp_rtp_packet->bufferPtr(), m_tmp_rtp_packet->bufferSize());
	m_tmp_rtp_packet->commit(num);

	if(num <= 0 ) {
		// rtp_packet->usedPayloadBufferSize(0);
		usleep(200);
	}
	else {

		switch( m_tmp_rtp_packet->payloadType() ) {
		case PAYLOAD_SYNC_OBJ:
		{
			m_tmp_sync_obj.deserialize(m_tmp_rtp_packet);
			LOG4CPLUS_INFO(m_timing_logger, "Received SyncObj: " << m_tmp_sync_obj);
			if(m_tmp_sync_obj.streamId() == m_media_stream_conn->syncRequestedForStreamID()) {
				// this sync object has been requested by the client. Use it immediately
				m_media_stream_conn->setRequestedSyncObj(m_tmp_rtp_packet);
				delete m_tmp_rtp_packet;
			}
			else {
				// beginning of next stream. put sync object into the packet ringbuffer
				m_packet_ringbuffer->appendRTPPacket(m_tmp_rtp_packet);
			}
			udp::endpoint ep = m_tmp_sync_obj.getMediaClockSrv();
			int tmp_port = ep.port();
			if(tmp_port != m_ts_port) {
				m_ts_port = tmp_port;
				CIPv4Address *sender = m_socket->latestSender();
				address_v4 tmp_addr(sender->sock_addr_in_ptr()->sin_addr.s_addr);
				address sender_addr(tmp_addr);
				m_media_stream_conn->useTimeService(sender_addr, m_ts_port);

			}

			// wake up playback thread
			//if(m_player->idleTime() > m_max_idle && m_max_idle != 0) {
			m_media_stream_conn->m_traffic_cond.Signal();
			//}

			break;
		}
		case PAYLOAD_PCM:
		case PAYLOAD_MP3:
		case PAYLOAD_VORBIS:
		case PAYLOAD_FLAC:
			// rtp_packet->BufferSize(num);
			m_packet_ringbuffer->appendRTPPacket(m_tmp_rtp_packet);
			// cerr << "Sender was: " << m_socket->latestSender()->ipAddress() << " port " << m_socket->latestSender()->port() << endl;

			// wake up playback thread
			//if(m_player->idleTime() > m_max_idle && m_max_idle != 0) {
			m_media_stream_conn->m_traffic_cond.Signal();
			//}

			break;
		default:
			cerr << "CRecvloop::DoLoop(): unknown payload type: " << m_tmp_rtp_packet->payloadType() << endl;

		}
		m_tmp_rtp_packet = 0;
	}

}


/*!
    \fn CRecvloop::sendPacket(CRTPPacket* packet)
 */
void CRecvloop::sendRTPPacket(CRTPPacket* packet)
{
	m_socket->sendTo(m_socket->latestSender(), packet->bufferPtr(), packet->usedBufferSize());
}
