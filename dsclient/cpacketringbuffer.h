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
#ifndef CPACKETRINGBUFFER_H
#define CPACKETRINGBUFFER_H

/**
ringbuffer implementation that takes RTP packets as input and audio frames as output. Therefore, the RTP header has an additional flag inidcation the start of frame packet and how many packet belong to that frame, so it can tell if the frame is complete. Furthermore, it indicates the beginning of a new stream, that is when audio parameters or the decoder may change and must be reinitialized.

@author Martin Runge
*/

#include <list>

#include "cmutex.h"

class CAudioFrame;
class CRTPPacket;

class CPacketRingBuffer{
public:
    CPacketRingBuffer(int num_of_frames);
    ~CPacketRingBuffer();
    
    void appendRTPPacket(CRTPPacket* packet);
    
    CRTPPacket* readPacket(void);

    int getRingbufferSize();
private:
  
    std::list<CRTPPacket*> m_packet_list;
  
    CMutex m_mutex;

    FILE* m_stream_fd;

    /** used to store the seqnum of the last read packet. At stream reset, this is set to -1, so packet nr 0 comes after it. */
    int m_seqnum;
};

#endif
