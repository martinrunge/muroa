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

// create a RTP packet object from a buffer. e.g. a RTP packet received via network
CRTPPacket::CRTPPacket(char *buffer, int buffer_size, bool delete_buffer_in_dtor )
{
  assert(buffer_size > sizeof(rtp_header_t));

  m_buffer = buffer;  
  m_buffer_size = buffer_size;
  m_used_payload_size = m_buffer_size - sizeof(rtp_header_t);

  m_delete_buffer_in_dtor = delete_buffer_in_dtor;

  init();
}

// create a new RTP packet object
CRTPPacket::CRTPPacket(int payload_size, bool delete_buffer_in_dtor) 
{
  m_buffer_size = payload_size + sizeof(rtp_header_t);
  m_buffer = new char[m_buffer_size];
  
  init();

  m_rtp_header->rtp_header_bits.version = 2;
  m_rtp_header->rtp_header_bits.padding = 0;
  m_rtp_header->rtp_header_bits.extension = 0;
  m_rtp_header->rtp_header_bits.CSRC_count = 0;
  m_rtp_header->rtp_header_bits.marker = 0;
  m_rtp_header->rtp_header_bits.sequence_number = 0;
  m_rtp_header->rtp_header_bits.timestamp = 0;

  // payload types:
  // 0 == sync object
  // 1 == PCM data
  m_rtp_header->rtp_header_bits.payload_type = PAYLOAD_UNKNOWN;

  m_used_payload_size = 0;

}


void CRTPPacket::init() {
  m_rtp_header = (rtp_header_t*)m_buffer;

  m_payload_ptr = m_buffer + sizeof(rtp_header_t);
  m_payload_size = m_buffer_size - sizeof(rtp_header_t);
  
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


/*!
    \fn CRTPPacket::operator=(CRTPPacket packet)
 */
CRTPPacket CRTPPacket::operator=(CRTPPacket packet)
{
    memcpy(m_buffer, packet.bufferPtr(), packet.bufferSize());
}
