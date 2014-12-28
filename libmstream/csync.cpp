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
#include "csync.h"
#include "crtppacket.h"

#include <netinet/in.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::asio::ip;


CSync::CSync(enum sync_type_t sync_type) : m_session_id(numeric_limits<uint32_t>::max()),
		                                   m_stream_id(numeric_limits<uint32_t>::max()),
		                                   m_frame_nr(numeric_limits<uint32_t>::max())
{
  cerr << "CSync::CSync(enum)" << endl;
  m_sync_type = sync_type;
  m_utc_time = new ptime(microsec_clock::universal_time());
  print();

}

CSync::CSync(CRTPPacket* rtp_packet)
{
  if(rtp_packet->payloadType() != PAYLOAD_SYNC_OBJ) return;

  cerr << "CSync::CSync(CRTPPacket*) (test) " ;
  m_utc_time = new ptime(from_iso_string("19700101T000000"));  // just a dummy to feed the c-tor. The value is never used!
  memcpy(m_serialization_buffer.raw_buffer, rtp_packet->payloadBufferPtr(), sizeof(m_serialization_buffer));
  deserialize();
  print();
}

CSync::~CSync()
{
  delete m_utc_time;
}


/*!
    \fn CSync::serialize()
 */
char* CSync::serialize()
{

  uint32_t tmp = m_sync_type;
  m_serialization_buffer.serialisation_vars.sync_type = htonl(tmp);
  m_serialization_buffer.serialisation_vars.stream_id = htonl(m_stream_id);
  m_serialization_buffer.serialisation_vars.session_id = htonl(m_session_id);

  m_serialization_buffer.serialisation_vars.frame_nr = htonl(m_frame_nr);

  strcpy((char*)m_serialization_buffer.serialisation_vars.timestamp, to_simple_string(*m_utc_time).c_str() );

  m_serialization_buffer.serialisation_vars.ip_port = m_media_clock_srv_endpoint.port();
  address addr = m_media_clock_srv_endpoint.address();

  if(addr.is_v6()) {
	  m_serialization_buffer.serialisation_vars.ip_proto_ver = 6;
	  address_v6 v6addr = addr.to_v6();
	  memcpy(m_serialization_buffer.serialisation_vars.ip_addr, v6addr.to_bytes().data(), v6addr.to_bytes().size());
  }
  else {
	  m_serialization_buffer.serialisation_vars.ip_proto_ver = 4;
	  address_v4 v4addr = addr.to_v4();
	  uint32_t* tmp_ptr = reinterpret_cast<uint32_t*>(m_serialization_buffer.serialisation_vars.ip_addr);
	  *tmp_ptr = htonl( v4addr.to_ulong() );
  }

  return m_serialization_buffer.raw_buffer;    
}


/*!
    \fn CSync::deserialize()
 */
void CSync::deserialize(void)
{
  uint32_t tmp = m_serialization_buffer.serialisation_vars.sync_type;
  m_sync_type = static_cast<sync_type_t>(ntohl(tmp));
  
  m_stream_id = ntohl( m_serialization_buffer.serialisation_vars.stream_id );
  m_session_id = ntohl( m_serialization_buffer.serialisation_vars.session_id );

  m_frame_nr = ntohl( m_serialization_buffer.serialisation_vars.frame_nr );

  *m_utc_time = time_from_string( m_serialization_buffer.serialisation_vars.timestamp );

  int port = m_serialization_buffer.serialisation_vars.ip_port;
  if(m_serialization_buffer.serialisation_vars.ip_proto_ver == 6) {
	  address_v6::bytes_type barray;
	  memcpy(barray.data(), &m_serialization_buffer.serialisation_vars.ip_addr, 16);
	  address_v6 v6addr(barray);

	  address ip_addr(v6addr);
	  m_media_clock_srv_endpoint = udp::endpoint(ip_addr, port);
  }
  else {
	  unsigned long* ipv4addr_ptr = reinterpret_cast<unsigned long*>(&m_serialization_buffer.serialisation_vars.ip_addr);
	  address_v4 v4addr(*ipv4addr_ptr);
	  address ip_addr(v4addr);
	  m_media_clock_srv_endpoint = udp::endpoint(ip_addr, port);
  }
}

udp::endpoint CSync::getMediaClockSrv() {
	return m_media_clock_srv_endpoint;
}

void CSync::setMediaClockSrv(udp::endpoint endpoint) {
	m_media_clock_srv_endpoint = endpoint;
}

void CSync::setMediaClockSrv(address ip_address, int port) {
	m_media_clock_srv_endpoint = udp::endpoint(ip_address, port);
}



/*!
    \fn CSync::serialisationBufferSize()
 */
int CSync::getSerialisationBufferSize()
{
    return sizeof(m_serialization_buffer);
}


/*!
    \fn CSync::getSerialisationBufferPtr
 */
char* CSync::getSerialisationBufferPtr() 
{
    return m_serialization_buffer.raw_buffer;
}


/*!
    \fn CSync::print()
 */
void CSync::print()
{
  cerr << "session/stream ID (" << sessionId() << "/" << streamId() << ") frame nr " 
       << frameNr() << " at " << *m_utc_time << endl;
}


/*!
    \fn CSync::addDuration(boost::posix_time::time_duration duration);
 */
void CSync::addDuration(boost::posix_time::time_duration duration)
{
  *m_utc_time += duration;
}


/*!
    \fn CSync::setTimeToNow()
 */
void CSync::setTimeToNow()
{
  *m_utc_time = microsec_clock::universal_time();
}


/*!
    \fn CSync::deserialize(RTPPacket* sync_packet)
 */
void CSync::deserialize(CRTPPacket* sync_packet)
{
  if(sync_packet->payloadType() != PAYLOAD_SYNC_OBJ) return;
  /* if(sync_packet->payloadBufferSize() != sizeof(m_serialization_buffer)) {
    cerr << "CSync::deserialize(CRTPPacket*): Warning: payload size does not fit. Ignoring packet!" << endl;
    return;
  }*/

  memcpy(m_serialization_buffer.raw_buffer, sync_packet->payloadBufferPtr(), sizeof(m_serialization_buffer));
  deserialize();
  // print();
}

ostream& operator<< (ostream &out, CSync &so)
{
    out << "session/stream ID (" << so.m_session_id << "/" << so.m_stream_id << ") frame nr "
         << so.frameNr() << " at " << *(so.m_utc_time);

    return out;
}
