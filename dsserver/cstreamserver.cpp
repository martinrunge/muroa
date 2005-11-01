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
#include "cstreamserver.h"

using namespace std;
using namespace boost::posix_time;


CStreamServer::CStreamServer(int session_id, int transport_buffer_size_in_ms) : 
            m_first_send_time(not_a_date_time), 
            m_send_time(not_a_date_time), 
            m_last_send_time(not_a_date_time) {

  m_session_id = session_id;
  m_stream_id = 0;

  m_payload_size = 1024;
  m_audio_bytes_per_frame = 2 * sizeof(short);

  m_transport_buffer_duration = millisec(transport_buffer_size_in_ms);

  m_payload_duration_sum = microseconds(0);
  m_total_play_time = microseconds(0);
  
  m_rtp_packet = new CRTPPacket(m_payload_size);


  m_frames_in_sync_period = 0;


}


CStreamServer::~CStreamServer()
{
}




/*!
    \fn CStreamServer::open()
 */
int CStreamServer::open(int audio_bytes_per_second)
{
  m_last_send_time = microsec_clock::local_time();
  m_last_payload_duration = not_a_date_time;
  
  ptime now = m_last_send_time;

  m_stream_id++;

  m_syncobj.addDuration(m_transport_buffer_duration);
  m_syncobj.frameNr(0);
  m_syncobj.streamId(m_stream_id);  
  m_syncobj.sessionId(m_session_id);
  m_syncobj.serialize();

  m_rtp_packet->payloadType(PAYLOAD_SYNC_OBJ);
  m_rtp_packet->copyInPayload(m_syncobj.getSerialisationBufferPtr(), m_syncobj.getSerialisationBufferSize());

  m_audio_bytes_per_second = audio_bytes_per_second;
  
  m_frames_in_sync_period = 0;
  m_payload_duration_sum = microseconds(0);

  sendToAllClients(m_rtp_packet);

  return 0;
}


/*!
    \fn CStreamServer::close()
 */
void CStreamServer::close()
{
    /// @todo implement me
    // send end of stream packet
}


/*!
    \fn CStreamServer::write(char* buffer, int length)
 */
int CStreamServer::write(char* buffer, int length) {

    time_duration payload_duration = microseconds((length * 1000000) / m_audio_bytes_per_second);

    m_rtp_packet->copyInPayload(buffer, length);
    

    m_rtp_packet->seqNum( m_rtp_packet->seqNum() + 1 ); 
    m_rtp_packet->timestamp( m_frames_in_sync_period );
    m_rtp_packet->payloadType(PAYLOAD_PCM);

//    measure the quality of the below usleep calculation
//    now = microsec_clock::local_time();
//    interval = now - m_last_send_time;
//    cerr << "interval: " << interval << endl;


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
      cerr << " Sleep duration was < 0!" << endl;
    }

    m_last_payload_duration = payload_duration;
}




/*!
    \fn CStreamServer::flush()
 */
void CStreamServer::flush()
{
    /// @todo implement me
}


list<CSocket*>::iterator CStreamServer::addSocket(CSocket* socket) {

  list<CSocket*>::iterator iter;

  // do some checks on the socket here.

  m_socket_list_mutex.Lock();
  iter = m_socket_list.insert(m_socket_list.end(), socket);
  m_socket_list_mutex.UnLock();

}


CSocket* CStreamServer::removeSocket(list<CSocket*>::iterator sock_iterator) {

  CSocket* socket;  

  socket = *sock_iterator;
  
  m_socket_list_mutex.Lock();
  m_socket_list.erase(sock_iterator);
  m_socket_list_mutex.UnLock();

  return socket;
}



/*!
    \fn CStreamServer::sendToAllClients(CRTPPacket* packet)
 */
void CStreamServer::sendToAllClients(CRTPPacket* packet)
{
    list<CSocket*>::iterator iter;

    for(iter = m_socket_list.begin(); iter != m_socket_list.end(); iter++ ) {
      (*iter)->write(packet->bufferPtr(), packet->usedBufferSize()); 
    }
}
