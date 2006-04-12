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
#include "crecvloop.h"
#include "caudioframe.h"
#include "csocket.h"
#include "csync.h"
#include "cpacketringbuffer.h"
#include "cplayer.h"

using namespace std;

CRecvloop::CRecvloop(CPlayer* parent, CPacketRingBuffer* packet_ringbuffer, unsigned short port): CThreadSlave()
{

  m_player = parent;
  m_packet_ringbuffer = packet_ringbuffer;

  m_socket = new CSocket(SOCK_DGRAM, port);  
  m_socket->recordSenderWithRecv(true);

  m_rtp_packet = new CRTPPacket();

}


CRecvloop::~CRecvloop()
{
  delete m_socket;
  delete m_rtp_packet;  
  
}


void CRecvloop::DoLoop()
{
  int num = m_socket->read(m_rtp_packet->bufferPtr(), m_rtp_packet->bufferSize()); 
  m_rtp_packet->commit(num);

  if(num <= 0 ) {
    // m_rtp_packet->usedPayloadBufferSize(0);
    usleep(200);
  }
  else {

    switch( m_rtp_packet->payloadType() ) {
      case PAYLOAD_SYNC_OBJ:
        m_player->setSyncObj(m_rtp_packet);

        // delete m_rtp_packet;
        break;


      case PAYLOAD_PCM:
      case PAYLOAD_MP3:
      case PAYLOAD_VORBIS:
      case PAYLOAD_FLAC:
        // m_rtp_packet->BufferSize(num);
        m_packet_ringbuffer->appendRTPPacket(m_rtp_packet);
        m_rtp_packet = new CRTPPacket(); 
        // cerr << "Sender was: " << m_socket->latestSender()->ipAddress() << " port " << m_socket->latestSender()->port() << endl;
        break;
      default:
        cerr << "CRecvloop::DoLoop(): unknown payload type: " << m_rtp_packet->payloadType() << endl; 
        
    }
  }
          

}


/*!
    \fn CRecvloop::sendPacket(CRTPPacket* packet)
 */
void CRecvloop::sendRTPPacket(CRTPPacket* packet)
{
    m_socket->sendTo(m_socket->latestSender(), packet->bufferPtr(), packet->usedBufferSize());
}
