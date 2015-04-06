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
#include "CStreamServer.h"
#include "CStreamCtrlConnection.h"
#include "CStreamConnection.h"

#include "cmds/CmdStreamReset.h"

#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>

#include <boost/asio.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace muroa;
using namespace log4cplus;

CStreamServer::CStreamServer(boost::asio::io_service& io_service, int timeServerPort, int session_id, int transport_buffer_size_in_ms) :
            m_first_send_time(not_a_date_time), 
            m_send_time(not_a_date_time), 
            m_last_send_time(not_a_date_time),
		    m_time_server_port(timeServerPort),
			m_io_service(io_service)
{

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

CStreamServer::~CStreamServer()
{
}


/*!
    \fn CStreamServer::open()
 */
int CStreamServer::open(int audio_bytes_per_second)
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
    \fn CStreamServer::close()
 */
void CStreamServer::close()
{
    cerr << "CStreamServer::close()" << endl;

    /// @todo implement me
    // send end of stream packet
}

/*!
    \fn CStreamServer::flush()
 */
void CStreamServer::flush()
{
	CmdStreamReset cmd_rst(m_session_id, m_stream_id, 0, 0);
	CRTPPacket *pkt = cmd_rst.toRTP();
	sendToAllClients(pkt);
}


int CStreamServer::write(char* buffer, int length) {
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
int CStreamServer::sendPacket(char* buffer, int length) {

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




list<CStreamCtrlConnection*>::iterator CStreamServer::addStreamConnection(CStreamCtrlConnection* conn) {

  list<CStreamCtrlConnection*>::iterator iter;

  // do some checks on the socket here.

  m_connection_list_mutex.Lock();
  iter = m_connection_list.insert(m_connection_list.end(), conn);
  m_connection_list_mutex.UnLock();
}


CStreamCtrlConnection* CStreamServer::removeStreamConnection(list<CStreamCtrlConnection*>::iterator conn_iterator) {

  CStreamCtrlConnection* conn;
  conn = *conn_iterator;
  
  m_connection_list_mutex.Lock();
  m_connection_list.erase(conn_iterator);
  m_connection_list_mutex.UnLock();

  return conn;
}



/*!
    \fn CStreamServer::sendToAllClients(CRTPPacket* packet)
 */
void CStreamServer::sendToAllClients(CRTPPacket* packet)
{
    list<CStreamCtrlConnection*>::iterator iter;
    m_connection_list_mutex.Lock();
    for(iter = m_connection_list.begin(); iter != m_connection_list.end(); iter++ ) {
      (*iter)->getStreamConnection()->send(packet->bufferPtr(), packet->usedBufferSize());
    }
    m_connection_list_mutex.UnLock();
}


/*!
    \fn CStreamServer::addClient(CIPv4Address* addr)
 */
list<CStreamCtrlConnection*>::iterator CStreamServer::addClient(bip::tcp::endpoint endp, const string& serviceName)
{
    bool known = false;

    list<CStreamCtrlConnection*>::iterator iter;
    for(iter = m_connection_list.begin(); iter != m_connection_list.end(); iter++ ) {
      bip::tcp::endpoint known_endp = (*iter)->remote_endpoint();
      if( known_endp == endp ) {
          known = true;
      }
    }

    if( known == false ) {
    	CStreamCtrlConnection* conn = new CStreamCtrlConnection(serviceName, this, m_io_service);
    	boost::system::error_code ec;
    	conn->connect(endp, ec);
    	if(ec) {

    	}
    	else {
    		conn->openStreamConnection();
    	}
//        return addStreamCtrlConnection(conn);
    }
    else {
        return m_connection_list.end();
    }
}

void CStreamServer::adjustReceiverList(std::vector<ServDescPtr> receivers)
{
    list<CStreamCtrlConnection*>::iterator iter;
    for(iter = m_connection_list.begin(); iter != m_connection_list.end(); iter++ ) {

        bool found = false;
        for(int i=0; i < receivers.size(); i++) {
            string servicename = receivers[i]->getServiceName();
            if( servicename.compare( (*iter)->getServiceName() ) == 0 ) {
                found = true;
                receivers.erase(receivers.begin() + i);
                break;
            }
        }
        if( found = false) {
            removeClient(iter);
        }
    }
    // add oaa receivers left in receivers list
    for(int i=0; i < receivers.size(); i++) {
        ServDescPtr srv_desc_ptr = receivers[i];
        bip::tcp::endpoint endp(bip::address::from_string(srv_desc_ptr->getHostName()), srv_desc_ptr->getPortNr() );
        addClient(endp, srv_desc_ptr->getServiceName());
    }
}

/*!
    \fn CStreamServer::removeClient(CIPv4Address* addr);
 */
void CStreamServer::removeClient(const string& name)
{
    list<CStreamCtrlConnection*>::iterator iter;
    for(iter = m_connection_list.begin(); iter != m_connection_list.end(); iter++ ) {
        CStreamCtrlConnection* sc = *iter;
        if( name.compare( sc->getServiceName() ) == 0 ) {
            removeClient(iter);
            iter--;
        }
    }
}


void CStreamServer::removeClient(list<CStreamCtrlConnection*>::iterator iter) {
  removeStreamConnection(iter);
}

/*!
    \fn CStreamServer::getSyncObj(uint32_t session_id, uint32_t stream_id)
 */
CSync* CStreamServer::getSyncObj(uint32_t session_id, uint32_t stream_id)
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
void CStreamServer::stdClientPort(int port)
{
  m_std_client_port = port;
}


/*!
    \fn CStreamServer::stdClientPort(void)
 */
int CStreamServer::stdClientPort(void)
{
  return m_std_client_port;
}


/*!
    \fn CStreamServer::listClients(void)
 */
void CStreamServer::listClients(void)
{
  list<CStreamCtrlConnection*>::iterator iter;
  cerr << "List of clients in session: " << endl;
  m_connection_list_mutex.Lock();
  for(iter = m_connection_list.begin(); iter != m_connection_list.end(); iter++ ) {
    cerr << (*iter)->remote_endpoint() << endl;
  }
  m_connection_list_mutex.UnLock();
}
