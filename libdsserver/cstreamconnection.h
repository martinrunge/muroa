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
#ifndef CSTREAMCONNECTION_H
#define CSTREAMCONNECTION_H

#include <csocket.h>
#include <crtppacket.h>
#include <csync.h>

#include <string>
/**
@author Martin Runge
*/

class CStreamServer;

class CStreamConnection
{
public:
    CStreamConnection(CStreamServer* parent, unsigned short bind_port = 0);

    ~CStreamConnection();
    int connect(CIPv4Address* addr);
    int send(char* buffer, int len);
    CIPv4Address* getClientAddress();

private:

    CSocket m_socket;
    CRTPPacket m_rtp_packet;
    CStreamServer* m_stream_server;
private:
    void handleReceivedPacket();
    CIPv4Address m_client_address;
};

#endif
