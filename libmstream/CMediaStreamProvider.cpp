/*
 * CMediaStreamProvider.cpp
 *
 *  Created on: 20.09.2015
 *      Author: martin
 */

#include "CMediaStreamProvider.h"
#include "CStreamConnection.h"

#include "cmds/CmdStreamReset.h"

#include <CApp.h>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>

#include <boost/asio.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace muroa;
using namespace log4cplus;

namespace muroa {

CMediaStreamProvider::CMediaStreamProvider(boost::asio::io_service& io_service, int session_id = 1, int transport_buffer_size_in_ms = 1500) :
                                           m_first_send_time(not_a_date_time),
                                           m_send_time(not_a_date_time),
                                           m_last_send_time(not_a_date_time),
        		                           m_time_server_port(12345),
        			                       m_io_service(io_service) {

	  m_timing_logger = Logger::getInstance("timing");

	  m_session_id = session_id;
	  m_stream_id = 0;

	  m_payload_size = 1024;
	  m_audio_bytes_per_frame = 2 * sizeof(short);

	  m_transport_buffer_duration = millisec(transport_buffer_size_in_ms);

	  m_payload_duration_sum = microseconds(0);
	  m_total_play_time = microseconds(0);

	  m_rtp_packet = new CRTPPacket(m_session_id, m_stream_id, m_payload_size);

	  m_frames_in_sync_period = 0;
}

CMediaStreamProvider::~CMediaStreamProvider() {
}

void CMediaStreamProvider::addJoinedConnection(CStreamCtrlConnection* conn) {
	m_connection_list_mutex.Lock();
	m_joined_connections.insert(conn);
	m_connection_list_mutex.UnLock();
}

void CMediaStreamProvider::removeJoinedConnection(CStreamCtrlConnection* conn) {
	m_connection_list_mutex.Lock();
	m_joined_connections.erase(conn);
	m_connection_list_mutex.UnLock();
}


/*!
    \fn CMediaStreamProvider::open()
 */
int CMediaStreamProvider::open(int audio_bytes_per_second)
{
  m_last_send_time = microsec_clock::universal_time();
  m_last_payload_duration = not_a_date_time;

  ptime now = m_last_send_time;

  m_stream_id++;

  m_syncobj.setTimeToNow();
  m_syncobj.addDuration(m_transport_buffer_duration);
  m_syncobj.frameNr(0);
  m_syncobj.streamId(m_stream_id);
  m_syncobj.sessionId(m_session_id);
  m_syncobj.setMediaClockSrv(boost::asio::ip::address(), m_time_server_port);
  m_syncobj.serialize();

  m_rtp_packet->sessionID(m_session_id);
  m_rtp_packet->streamID(m_stream_id);

  m_rtp_packet->seqNum(m_rtp_packet->seqNum() + 1);
  m_rtp_packet->payloadType(PAYLOAD_SYNC_OBJ);
  m_rtp_packet->copyInPayload(m_syncobj.getSerialisationBufferPtr(), m_syncobj.getSerialisationBufferSize());

  m_audio_bytes_per_second = audio_bytes_per_second;

  m_frames_in_sync_period = 0;
  m_payload_duration_sum = microseconds(0);
  cerr << "CStreamServer::open: audiobytes/s = " << audio_bytes_per_second
       << " session/stream id = (" << m_session_id << "/" << m_stream_id << ")"
       << " time = " << now << endl;

  LOG4CPLUS_DEBUG(m_timing_logger, "open stream " << m_transport_buffer_duration.total_milliseconds() << " ms." );

  sendToAllClients(m_rtp_packet);

  return 0;	LOG4CPLUS_DEBUG(m_timing_logger, "sync");
}


/*!
    \fn CMediaStreamProvider::close()
 */
void CMediaStreamProvider::close()
{
    cerr << "CStreamServer::close()" << endl;

    /// @todo implement me
    // send end of stream packet
}

/*!
    \fn CStreamServer::flush()
 */
void CMediaStreamProvider::flush()
{
	CmdStreamReset cmd_rst(m_session_id, m_stream_id, 0, 0);
	CRTPPacket *pkt = cmd_rst.toRTP();
	sendToAllClients(pkt);
}


int CMediaStreamProvider::write(char* buffer, int length) {
    int sum = 0;
    int left = length;
    int offset = 0;

   while( left > 0 ) {
       if(left >= 1024) {
           sum += sendPacket(buffer + offset, 1024);
           offset += 1024;
           left -= 1024;
       }
       else {
           sum += sendPacket(buffer + offset, left);
           offset += left;
           left = 0;
       }
   }
   return sum;
}

/*!
    \fn CStreamServer::write(char* buffer, int length)
 */
int CMediaStreamProvider::sendPacket(char* buffer, int length) {

    time_duration payload_duration = microseconds((length * 1000000) / m_audio_bytes_per_second);

    m_rtp_packet->copyInPayload(buffer, length);


    m_rtp_packet->seqNum( m_rtp_packet->seqNum() + 1 );
    m_rtp_packet->timestamp( m_frames_in_sync_period );
    m_rtp_packet->payloadType(PAYLOAD_PCM);

//    measure the quality of the below usleep calculation

    //now = microsec_clock::universal_time();
    //interval = now - m_last_send_time;
    //cerr << "interval: " << interval << endl;


    sendToAllClients(m_rtp_packet);

    m_frames_in_sync_period += length / m_audio_bytes_per_frame;

//    m_last_send_time = now;

    if(m_last_payload_duration.is_special() == true) {
      // if this is the first packet, assume the "last" with same duration
      // for sleep calculation below.

      cerr << " CStreamSource::readAndSend: ============ STREAM RESET! =============" << endl;
      m_last_payload_duration = payload_duration;
      m_time_since_last_packet = m_last_payload_duration;

      m_first_send_time = microsec_clock::universal_time();
      m_send_time = m_first_send_time;

      m_payload_duration_sum = payload_duration;
      // cerr << " payload_duration sum = " << m_payload_duration_sum;

    }
    else {
      m_send_time = microsec_clock::universal_time();
      m_payload_duration_sum += payload_duration;
      // cerr << " payload_duration sum=" << m_payload_duration_sum;
    }

    m_total_play_time = m_send_time - m_first_send_time;
    // cerr << " total play time=" << total_play_time;

    time_duration sleep_duration = m_payload_duration_sum - m_total_play_time;
    long sleep_duration_in_us = sleep_duration.total_microseconds();

    // cerr << " -> sleeping " << sleep_duration_in_us << " us before sending next packet." << endl;

    // only sleep, if there is time to sleep. especially on negative values, we are much too late, so don't sleep at all.
    if(sleep_duration_in_us > 0)
      usleep(sleep_duration_in_us);
    else {
      // cerr << " Sleep duration was < 0!" << endl;
    }

    m_last_payload_duration = payload_duration;
}

/*!
    \fn CStreamServer::sendToAllClients(CRTPPacket* packet)
 */
void CMediaStreamProvider::sendToAllClients(CRTPPacket* packet)
{
    set<CStreamCtrlConnection*>::iterator iter;
    m_connection_list_mutex.Lock();
    for(iter = m_joined_connections.begin(); iter != m_joined_connections.end(); iter++ ) {
      (*iter)->getStreamConnection()->send(packet->bufferPtr(), packet->usedBufferSize());
    }
    m_connection_list_mutex.UnLock();
}



/*!
    \fn CStreamServer::getSyncObj(uint32_t session_id, uint32_t stream_id)
 */
CSync* CMediaStreamProvider::getSyncObj(uint32_t session_id, uint32_t stream_id)
{
    if(m_syncobj.sessionId() == session_id && m_syncobj.streamId() == stream_id) {
      m_syncobj.serialize();
      return &m_syncobj;
    }
    else {
      return 0;
    }
}


/*!
    \fn CStreamServer::stdClientPort(int port)
 */
void CMediaStreamProvider::stdClientPort(int port)
{
  m_std_client_port = port;
}


/*!
    \fn CStreamServer::stdClientPort(void)
 */
int CMediaStreamProvider::stdClientPort(void)
{
  return m_std_client_port;
}


/*!
    \fn CStreamServer::listClients(void)
 */
void CMediaStreamProvider::listClients(void)
{
  set<CStreamCtrlConnection*>::iterator iter;
  cerr << "List of clients in session: " << endl;
  m_connection_list_mutex.Lock();
  for(iter = m_joined_connections.begin(); iter != m_joined_connections.end(); iter++ ) {
    cerr << (*iter)->remote_endpoint() << endl;
  }
  m_connection_list_mutex.UnLock();
}


} /* namespace muroa */
