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
#include "cringbuffer.h"
#include "caudioframe.h"
#include "crtppacket.h"

#include <iostream>

using namespace std;

CRingBuffer::CRingBuffer(int num_of_frames)
{
  m_stream_fd = fopen("debug_file_client.raw", "w");
  m_seqnum = 0;
}


CRingBuffer::~CRingBuffer()
{
  fclose(m_stream_fd);
}


CRTPPacket* CRingBuffer::readPacket(void) {
     
    int ringbuffer_size;

    m_mutex.Lock();
    ringbuffer_size = m_packet_list.size();
    if(ringbuffer_size == 0) {
      m_mutex.UnLock();
      cerr << "CRingBuffer::readFrame: buffer empty!" << endl;
      return 0;
    }
    

    CRTPPacket* rtp_packet = *m_packet_list.begin();
    m_packet_list.pop_front();

    m_mutex.UnLock();

    int seqnum = rtp_packet->seqNum();
    if(seqnum != m_seqnum + 1) {
      cerr << "CPlayloop::DoLoop: last packet: " << m_seqnum
           << " new RTP seqnum " << seqnum 
           << "! " << seqnum - m_seqnum 
          << " packets missing!" << endl;
    }
    m_seqnum = seqnum;

    
//    if(first_packet_of_frame) {
//      CAudioFrame* frame = new CAudioFrame(payload_type, payload_size);
//      frame->copyData(payload_data, payload_size);
//      m_framelist.push_back(frame);
//    }
//    else {
//      cerr << "CRingBuffer::commitRtpPacket: got packet which is not first in frame" << endl;  
//    }
  
    return rtp_packet;    
}


void CRingBuffer::appendRTPPacket(CRTPPacket* packet)
{
  m_mutex.Lock();
  m_packet_list.push_back(packet);    
  m_mutex.UnLock();
}

int CRingBuffer::getRingbufferSize() {
  int size;

  m_mutex.Lock();
  size = m_packet_list.size();
  m_mutex.UnLock();
  
  return size;
}