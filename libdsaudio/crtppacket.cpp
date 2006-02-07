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
#include "crtppacket.h"

#include <string.h>
#include <assert.h>

const unsigned short CRTPPacket::DS_RTP_PROFILE = 0x2222;

// create a RTP packet object from a buffer. e.g. a RTP packet received via network
CRTPPacket::CRTPPacket(char *buffer, int buffer_size, bool delete_buffer_in_dtor )
{
  assert(buffer_size > sizeof(rtp_header_t));

  m_buffer = buffer;  
  m_buffer_size = buffer_size;
  int total_header_size;


  m_rtp_header = reinterpret_cast<rtp_header_t*>(m_buffer);
  m_num_csrc = m_rtp_header->rtp_header_bits.CSRC_count;
  if(m_num_csrc > 15) m_num_csrc = 15;
    

  total_header_size = sizeof(rtp_header_t) + m_num_csrc * sizeof(unsigned long);


  if(m_rtp_header->rtp_header_bits.extension == 1) {
    total_header_size += sizeof(rtp_header_extension_t);
  }
  

  m_used_payload_size = m_buffer_size - total_header_size;

  m_delete_buffer_in_dtor = delete_buffer_in_dtor;

  init();
}

// create a new RTP packet object
CRTPPacket::CRTPPacket(unsigned long session_id, unsigned long stream_id, int payload_size, bool delete_buffer_in_dtor) {

  m_num_csrc = 0;

  m_buffer_size = payload_size + sizeof(rtp_header_t);
  m_buffer_size += m_num_csrc * sizeof(unsigned long);
  m_buffer_size += sizeof(rtp_header_extension_t);

  m_buffer = new char[m_buffer_size];
  m_rtp_header = reinterpret_cast<rtp_header_t*>(m_buffer);
 
  m_rtp_header->rtp_header_bits.version = 2;
  m_rtp_header->rtp_header_bits.padding = 0;
  m_rtp_header->rtp_header_bits.extension = 1;
  m_rtp_header->rtp_header_bits.CSRC_count = 0;
  m_rtp_header->rtp_header_bits.marker = 0;
  m_rtp_header->rtp_header_bits.sequence_number = 0;
  m_rtp_header->rtp_header_bits.timestamp = 0;

  init();

  // payload types:
  // 0 == sync object
  // 1 == PCM data
  m_rtp_header->rtp_header_bits.payload_type = PAYLOAD_UNKNOWN;

  m_rtp_header_extension->defined_by_profile = CRTPPacket::DS_RTP_PROFILE;
  m_rtp_header_extension->num_32bit_words_following = (sizeof(rtp_header_extension_t) - 2 * sizeof(unsigned short)) / sizeof(unsigned long);
  m_rtp_header_extension->ds_session_id = session_id;
  m_rtp_header_extension->ds_stream_id = stream_id;



  m_used_payload_size = 0;

}


void CRTPPacket::init() {
  int total_header_size;  

  m_rtp_header = reinterpret_cast<rtp_header_t*>(m_buffer);

  total_header_size = sizeof(rtp_header_t) + m_num_csrc * sizeof(unsigned long);
  char *tmp = m_buffer + total_header_size;

  if(m_rtp_header->rtp_header_bits.extension) {
    m_rtp_header_extension = reinterpret_cast<rtp_header_extension_t*>(tmp);
    total_header_size += sizeof(rtp_header_extension_t);
  }
  else {
    m_rtp_header_extension = 0;
  }

  m_payload_ptr = m_buffer + total_header_size;
  m_payload_size = m_buffer_size - total_header_size;
  
}

int CRTPPacket::commit(int num) {

  int total_header_size;


  m_rtp_header = reinterpret_cast<rtp_header_t*>(m_buffer);
  m_num_csrc = m_rtp_header->rtp_header_bits.CSRC_count;
  if(m_num_csrc > 15) m_num_csrc = 15;
    

  total_header_size = sizeof(rtp_header_t) + m_num_csrc * sizeof(unsigned long);


  if(m_rtp_header->rtp_header_bits.extension == 1) {
    total_header_size += sizeof(rtp_header_extension_t);
  }
  
  if(num > total_header_size)
  	m_used_payload_size = num - total_header_size;
  else
	  m_used_payload_size = 0;

  init();

  return 0;
}



CRTPPacket::~CRTPPacket()
{
  if(m_delete_buffer_in_dtor) 
  {
    delete [] m_buffer;  
  }
}


int CRTPPacket::copyInPayload(const char* from, const int size)
{
  assert( m_payload_size >= size );
  memcpy(m_payload_ptr, from, size); 
  m_used_payload_size = size;
}

void CRTPPacket::usedPayloadBufferSize(int used)
{
  assert(used <= m_payload_size);
  m_used_payload_size = used;
}


void CRTPPacket::usedBufferSize(int size)
{  
  if(size >= sizeof(rtp_header_t))
  {
    m_used_payload_size = size - sizeof(rtp_header_t); 
  }
  else 
  {
    m_used_payload_size = 0;
  }
}


int CRTPPacket::usedBufferSize(void) { 
  int used_size = m_used_payload_size;
  used_size += sizeof(rtp_header_t);
  used_size += m_num_csrc * sizeof(unsigned long);

  if(m_rtp_header->rtp_header_bits.extension == 1) {
    used_size += sizeof(rtp_header_extension_t);
  }

  return used_size; 

}

/*!
    \fn CRTPPacket::operator=(CRTPPacket packet)
 */
CRTPPacket CRTPPacket::operator=(CRTPPacket packet)
{
    memcpy(m_buffer, packet.bufferPtr(), packet.bufferSize());
}


unsigned long CRTPPacket::sessionID(void) {
  if(m_rtp_header->rtp_header_bits.extension == 1) {
    return m_rtp_header_extension->ds_session_id;
  }
  else {
    return 0;
  }
}

void CRTPPacket::sessionID(unsigned long session_id){
  if(m_rtp_header->rtp_header_bits.extension == 1) {
    m_rtp_header_extension->ds_session_id = session_id;
  }
}

unsigned long CRTPPacket::streamID(void){
  if(m_rtp_header->rtp_header_bits.extension == 1) {
    return m_rtp_header_extension->ds_stream_id;
  }
  else {
    return 0;
  }

}

void CRTPPacket::streamID(unsigned long stream_id){
  if(m_rtp_header->rtp_header_bits.extension == 1) {
    m_rtp_header_extension->ds_stream_id = stream_id;
  }
}
