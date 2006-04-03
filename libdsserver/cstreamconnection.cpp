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
#include "cstreamconnection.h"
#include "cstreamserver.h"

#include <iostream>

using namespace std;

CStreamConnection::CStreamConnection(CStreamServer* parent, unsigned short bind_port) : m_socket(SOCK_DGRAM, bind_port )
{
  m_stream_server = parent;
  m_socket.setNonBlocking(0);
  m_socket.recordSenderWithRecv(false);
}

CStreamConnection::~CStreamConnection()
{

}




/*!
    \fn CStreamConnection::connect(CIPv4Address* addr)
 */
void CStreamConnection::connect(CIPv4Address* addr)
{
    m_socket.connect(addr);
}


/*!
    \fn CStreamConnection::send(char* buffer, int len)
 */
int CStreamConnection::send(char* buffer, const int len)
{
    // 1) send data away
    int num = m_socket.write(buffer, len);
  
    // 2) check, if there was a rtp packet sent from the client
    int read_num = m_socket.read(m_rtp_packet.bufferPtr(), m_rtp_packet.bufferSize());   

    // cerr << "m_socket.read ret " << read_num << endl;
    if(read_num > 0) {
      handleReceivedPacket();      
    }

    return num;
}


/*!
    \fn CStreamConnection::handleReceivedPacket()
 */
void CStreamConnection::handleReceivedPacket()
{
    cerr << "CStreamConnection::handleReceivedPacket: received a RTP packet from the client." << endl;
    cerr << "RTP: payloadType = " << m_rtp_packet.payloadType() 
         << " sessionID = " << m_rtp_packet.sessionID() 
         << " streamID = " << m_rtp_packet.streamID() << endl; 

    if(m_rtp_packet.payloadType() == PAYLOAD_SYNC_OBJ) {
      CSync tmp_sync(&m_rtp_packet);
      tmp_sync.deserialize();

      if(tmp_sync.syncType() == SYNC_REQ_STREAM) {
        // the clients needs a sync object for this stream !!!
        CSync* session_sync_obj;
        session_sync_obj = m_stream_server->getSyncObj(tmp_sync.sessionId(), tmp_sync.streamId());
        if(session_sync_obj != 0) {

          CRTPPacket packet(session_sync_obj->sessionId(), session_sync_obj->streamId(), sizeof(CSync), true);

          session_sync_obj->serialize();
          packet.copyInPayload(session_sync_obj->getSerialisationBufferPtr(), session_sync_obj->getSerialisationBufferSize());
      
          packet.payloadType(PAYLOAD_SYNC_OBJ);

          m_socket.write(packet.bufferPtr(), packet.usedBufferSize());        
        }
      }  

    } 

}
