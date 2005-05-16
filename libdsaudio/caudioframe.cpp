/***************************************************************************
 *   Copyright (C) 2004 by Martin Runge                                    *
 *   martin.runge@web.de                                                   *
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
#include <iostream>

#include <assert.h>

#include "caudioframe.h"
#include "crtppacket.h"

using namespace std;


CAudioFrame::CAudioFrame(enum payload_type_t payload_type, int max_frame_size) {
  m_frame_type = payload_type;
  m_max_size = max_frame_size;
  m_frame_data = new char[m_max_size];

  // nothing is used yet
  m_frame_data_size = 0;  
}

CAudioFrame::CAudioFrame(CRTPPacket* rtp_packet, int max_frame_size) {
  m_frame_type = rtp_packet->payloadType();
  m_frame_data_size = rtp_packet->payloadBufferSize();
  m_max_size = max_frame_size;
  m_frame_data = new char[m_max_size];

  copyData(rtp_packet->payloadBufferPtr(), rtp_packet->payloadBufferSize());

  m_first_sample_nr = rtp_packet->timestamp(); 
}


CAudioFrame::~CAudioFrame() {
  delete m_frame_data;
}


char* CAudioFrame::dataEndPtr(void) {
  return m_frame_data + m_frame_data_size;
}

void CAudioFrame::dataSizeAdded(int num_additional_bytes) {
  assert(num_additional_bytes + m_frame_data_size < m_max_size);

  m_frame_data_size += num_additional_bytes;
}


void CAudioFrame::copyData(const char* data, const int data_len) {
  if(data_len > m_max_size) {
    cerr << "CAudioFrame::copyData: ERROR! Block of size " << data_len << " exceeds max size (" << m_max_size << ")!" << endl;
    return;
  }
  memcpy(m_frame_data, data, data_len);
  m_frame_data_size = data_len;
  
}

void CAudioFrame::moveDataToBegin(const int from) {

  if(from > 0) {
    assert(from * 2 > m_frame_data_size);

    char* start_ptr = m_frame_data + from;  

    memcpy(m_frame_data, start_ptr, m_frame_data_size - from);
    m_frame_data_size -= from;
  }
}

