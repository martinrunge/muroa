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
#include <iostream>

#include <cstdlib>
#include <string.h>

#include <sys/stat.h> 

#include "cpthread.h"
#include "libsock++.h"
#include "libdsaudio.h"

#include "caudioframe.h"
#include "csocket.h"

#include "cplayloop.h"
#include "crecvloop.h"
#include "CApp.h"
#include "CSettings.h"
#include "CTcpServer.h"
#include "CTimeServiceCtrl.h"


using namespace std;
using namespace boost::posix_time;
using namespace muroa;

CMediaStreamConnection::CMediaStreamConnection(boost::asio::io_service& io_service, boost::asio::ip::address mcast_addr, int timesrv_port ) :
		                   m_max_sync_infos(10),
		                   m_io_service(io_service),
						   m_mcast_addr(mcast_addr),
						   m_timesrv_port(timesrv_port)
{
  
  m_packet_ringbuffer = new CPacketRingBuffer(3);

  m_recvloop = new CRecvloop(this, m_packet_ringbuffer);
  m_playloop = new CPlayloop(this, m_packet_ringbuffer);

  m_recvloop_thread = new CPThread(m_recvloop);
  m_playloop_thread = new CPThread(m_playloop);

  m_sync_requested_for_stream_id = -1;
  m_sync_requested_at = microsec_clock::universal_time();


  m_idle_time = 0;

  m_ts = new CTimeServiceCtrl();
  //CSync syncobj;

  start();
}


CMediaStreamConnection::~CMediaStreamConnection()
{
  stop();
  m_ts->stop();
  delete m_ts;
  delete m_recvloop_thread;
  delete m_playloop_thread;

  delete m_recvloop;
  delete m_playloop;

  delete m_packet_ringbuffer;

}


/*!
    \fn CPlayer::start()
 */
void CMediaStreamConnection::start()
{
  m_recvloop_thread->StartThread();

  if(!m_sync_info_queue.empty()) {
	  m_playloop_thread->StartThread(true);
  }
}

/*!
    \fn CPlayer::stop()
 */
void CMediaStreamConnection::stop()
{
  m_recvloop_thread->StopThread();
  m_playloop_thread->StopThread();
}

const bool CMediaStreamConnection::multicastSupported() const {
	/// TODO: check here, if recvloop was able to join to multicast group
	return false;
}


int CMediaStreamConnection::getRTPUnicastPort() {
	return m_recvloop->getRTPPort();
}


/*!
    \fn CPlayer::sendSyncReq(CSync* sync_req)
 */
void CMediaStreamConnection::sendRTPPacket(CRTPPacket* packet)
{
    m_recvloop->sendRTPPacket(packet);
}


muroa::evSyncStream CMediaStreamConnection::getSyncInfo() {
	lock_guard<mutex> lg(m_sync_info_mutex);

	assert( !m_sync_info_queue.empty() );

	return *m_sync_info_queue.front();
}


void CMediaStreamConnection::onSyncInfo(const evSyncStream& evt) {
	// make a copy
	muroa::evSyncStream* sync_info = new evSyncStream(evt);
	m_sync_info_queue.push(sync_info);

	if(m_sync_info_queue.size() >= m_max_sync_infos) {
		muroa::evSyncStream* oldest_sync_info = m_sync_info_queue.front();
		delete oldest_sync_info;
		m_sync_info_queue.pop();
	}

	m_playloop_thread->StartThread(true);
}


/*!
    \fn CPlayer::setRequestedSyncObj(CRTPPacket* rtp_packet)
 */
void CMediaStreamConnection::setRequestedSyncObj(CRTPPacket* rtp_packet)
{
    m_sync_requested_for_stream_id = -1;
    // setSyncObj(rtp_packet);
}


/*!
    \fn CPlayer::requestSync(int stream_id)
 */
void CMediaStreamConnection::requestSync(int session_id, int stream_id)
{
  if( m_sync_requested_for_stream_id == -1) {  // no request is underway yet  
    CSync *sync_req = new CSync(SYNC_REQ_STREAM);
    sync_req->sessionId(session_id);
    sync_req->streamId(stream_id);
    sync_req->serialize();

    cerr << "sending request for sync obj." << endl;

    CRTPPacket* tmp_packet = new CRTPPacket(session_id, stream_id, sync_req->getSerialisationBufferSize(), true);

    tmp_packet->copyInPayload(sync_req->getSerialisationBufferPtr(), sync_req->getSerialisationBufferSize());
      
    tmp_packet->payloadType(PAYLOAD_SYNC_OBJ);
    sendRTPPacket(tmp_packet);

    m_sync_requested_for_stream_id = stream_id;
    m_sync_requested_at = microsec_clock::universal_time();

    delete sync_req;
    delete tmp_packet;     
  }
}

void CMediaStreamConnection::onResetStream(const evResetStream& evRst) {
    cerr << "CPlayer::onResetStream: sessionID: " << evRst.m_ssrc << endl;

	lock_guard<mutex> lg(m_sync_info_mutex);
	if(!m_sync_info_queue.empty()) {
		muroa::evSyncStream* si = m_sync_info_queue.front();
		if( si->m_ssrc == evRst.m_ssrc) {
        	delete m_sync_info_queue.front();
        	m_sync_info_queue.pop();
    	}
    	m_packet_ringbuffer->clear( evRst.m_ssrc );
    	m_playloop->resetStream( evRst.m_ssrc );
    }
    else {
    	LOG4CPLUS_WARN(CApp::logger(), "got Reset Stream for ssrc that is not currently active. Ignoring it.");
    }
}



void CMediaStreamConnection::useTimeService(boost::asio::ip::address ip_address, int port, boost::asio::ip::udp protocol) {
	m_ts->start(false, ip_address, port, protocol);
}
