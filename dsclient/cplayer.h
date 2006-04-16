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
#ifndef CPLAYER_H
#define CPLAYER_H

/**
Class encapsulates all the playback functioinalty. It inplements an interface to the res of a media player, so it can easyly be serperated from GUI functions like playlist handling.

@author Martin Runge
*/

#include <string>
#include "csync.h"

class CRecvloop;
class CPlayloop;
class CRTPPacket;
class CPThread;
class CPacketRingBuffer;

class CPlayer{
public:
    CPlayer(unsigned short port, std::string sound_dev);

    ~CPlayer();
    void start();
    void stop();
    void sendRTPPacket(CRTPPacket* packet);

    inline CSync* syncObj() {return &m_sync_obj; };
    void setSyncObj(CRTPPacket* rtp_packet);
    void sync();

    inline int syncRequestedForStreamID(void) { 
      return m_sync_requested_for_stream_id;
    };

    void requestSync(int session_id, int stream_id);
    void setRequestedSyncObj(CRTPPacket* rtp_packet);

private:
    CPacketRingBuffer * m_packet_ringbuffer;
  
    CPlayloop *m_playloop;
    CRecvloop *m_recvloop;

    CPThread *m_recvloop_thread;
    CPThread *m_playloop_thread;

    CSync m_sync_obj;

    int m_sync_requested_for_stream_id;
    boost::posix_time::ptime m_sync_requested_at;

};

#endif
