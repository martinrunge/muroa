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
#ifndef CRECVLOOP_H
#define CRECVLOOP_H

#include <cthreadslave.h>

#include "csync.h"
#include "cmuroad.h"
/**
@author Martin Runge
*/

class CRTPPacket;
class CSocket;
class CPacketRingBuffer;
class CPlayer;

class CRecvloop : public CThreadSlave
{
public:
    CRecvloop(CPlayer* parent, Cmuroad *config, CPacketRingBuffer* packet_ringbuffer);

    ~CRecvloop();
  
    void DoLoop();
    void sendRTPPacket(CRTPPacket* packet);

private:
    CRTPPacket *m_rtp_packet;
    CSocket *m_socket;

    CPacketRingBuffer *m_packet_ringbuffer;
    CPlayer* m_player;
    Cmuroad* m_config;
    CSync m_tmp_sync_obj;
};

#endif
