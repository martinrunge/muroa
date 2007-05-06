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
#include "cpacketringbuffer.h"
#include "caudioframe.h"
#include "crtppacket.h"

#include <iostream>

using namespace std;

CPacketRingBuffer::CPacketRingBuffer(int num_of_frames)
{
  //m_stream_fd = fopen("debug_file_client.raw", "w");
  m_seqnum = -1;
}


CPacketRingBuffer::~CPacketRingBuffer()
{
  //fclose(m_stream_fd);
  while(!m_packet_list.empty()) {
    delete *m_packet_list.begin();
    m_packet_list.pop_front();
  }
}


CRTPPacket* CPacketRingBuffer::readPacket(void) {
     
    int ringbuffer_size;

    m_mutex.Lock();
    ringbuffer_size = m_packet_list.size();
    if(ringbuffer_size == 0) {
      m_mutex.UnLock();
      // cerr << "CRingBuffer::readPacket: buffer empty!" << endl;
      return 0;
    }
    

    CRTPPacket* rtp_packet = *m_packet_list.begin();
    m_packet_list.pop_front();


    int seqnum = rtp_packet->seqNum();
    if(seqnum != m_seqnum + 1) {
      cerr << "CPlayloop::DoLoop: last packet: " << m_seqnum
           << " new RTP seqnum " << seqnum 
           << "! " << seqnum - m_seqnum 
          << " packets missing!" << endl;
    }
    m_seqnum = seqnum;

    //cerr << "CRingBuffer::readPacket: one packet read from list beginning " << endl;
    m_mutex.UnLock();
    
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


void CPacketRingBuffer::appendRTPPacket(CRTPPacket* packet)
{
  int seqnum = packet->seqNum();

  // Handle overflow of sequence numbers:
  // sequence number is a 16 bint unsigned -> 0...65335
  // if seqnum of new packet is near 0 AND seqnum of last read packet is near 65335
  // the sequence number probably hat an overflow at 65335, so seqnum 0 is one after 65335.
  // in that case, add 65335 to the seqnum of the packet to insert and serach its position. 
  if(seqnum < (1 << 14) && m_seqnum > (1 << 14)) {
    seqnum += (1 << 16);
  }

  m_mutex.Lock();

  std::list<CRTPPacket*>::iterator iter_post = m_packet_list.end(); 
  std::list<CRTPPacket*>::iterator iter_pre;

  do {
    if( iter_post == m_packet_list.begin() ) {  // we arrived at the beginning of the list or the list is empty

      if(m_packet_list.size() == 0) {  //list is empty
        // cerr << "CPacketRingBuffer::appendRTPPacket: list empty" << endl;
        if(m_seqnum < seqnum) { // insert only, if the sequence number is higher that the last read packet
          // cerr << "CPacketRingBuffer::appendRTPPacket: dropping packet. seqnum < m_seqnum." << endl;
          m_packet_list.push_back(packet);
        }
        else {
          cerr << "CPacketRingBuffer::appendRTPPacket: dropping packet for empty list: seqnr " << seqnum << ". Last read was: " <<  m_seqnum << endl;
          delete packet;  
        }
        break;
      }

      else {  // iterated to the beginning
        if(m_seqnum < seqnum) { // insert only, if the sequence number is higher that the last read packet
          m_packet_list.insert(iter_post, packet); 
          // cerr << "CPacketRingBuffer::appendRTPPacket: packet inserted at beginning" << endl;
        }
        else {
           cerr << "CPacketRingBuffer::appendRTPPacket: dropping packet at begin: seqnr " << seqnum << ". Last read was: " <<  m_seqnum << endl;
           delete packet;
        }
        break;
      }      
    }
    else { // still searching for the position
      iter_pre = iter_post;
      --iter_pre;
      if((*iter_pre)->seqNum() < seqnum) {
        m_packet_list.insert(iter_post, packet);   
        // cerr << "CPacketRingBuffer::appendRTPPacket: packet inserted in the middle" << endl;
        break;
      }
      if((*iter_pre)->seqNum() == packet->seqNum()) { // duplicate packet! drop!
        cerr << "CPacketRingBuffer::appendRTPPacket: dropping duplicate packet: seqnr " << seqnum << endl;
        delete packet;
        break;
      }
      --iter_post;
    }
  } while(1);


  m_mutex.UnLock();
}

int CPacketRingBuffer::getRingbufferSize() {
  int size;

  m_mutex.Lock();  
  size = m_packet_list.size();
  // cerr << "CPacketRingBuffer::getRingbufferSize() = " << size << endl;
  m_mutex.UnLock();
  
  return size;
}
