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
#ifndef CSTREAMSERVER_H
#define CSTREAMSERVER_H

/**
Class provides a server for a stream.

@author Martin Runge
*/

#include <iostream>
#include <list>

#include <sys/time.h>

#include <boost/date_time/posix_time/posix_time.hpp>

// #include "libsock++.h"
// #include "libdsaudio.h"

#include "cstreamconnection.h"
#include "csync.h"
#include "cmutex.h"
#include "crtppacket.h"




class CStreamServer{
public:
    CStreamServer(int session_id = 1, int transport_buffer_size_in_ms = 1500);

    ~CStreamServer();

    int open(int audio_bytes_per_second = 2 * 2 * 44100);

    void close();

    int write(char* buffer, int length);

    void flush();

    std::list<CStreamConnection*>::iterator addStreamConnection(CStreamConnection* conn);
    CStreamConnection* removeStreamConnection(std::list<CStreamConnection*>::iterator conn_iterator);

    void removeClient(CIPv4Address* addr);
    void removeClient(std::list<CStreamConnection*>::iterator iter);
    std::list<CStreamConnection*>::iterator addClient(CIPv4Address* addr);
    CSync* getSyncObj(uint32_t session_id, uint32_t stream_id);

    /*!
      \fn CStreamServer::adjustClientListTo(std::vector<std::string> clients)
      \brief  specify a new list of clients and adjust the used list to that new list.

      This is done in two steps: first, search the connection list for cliet connections not listed any more in the new list and remove them. Second, search the new list for clients, that are not yet listed in the modified connection list and add them.
    */
    void adjustClientListTo(std::list<std::string> clients);

    void stdClientPort(int port);
    int stdClientPort(void);

private:
    unsigned long m_num;
    int m_payload_size;

    int m_num_clients;

    int m_audio_bytes_per_second;
    int m_audio_bytes_per_frame;

    long m_frames_in_sync_period;

    /** system time when the first packet was sent */
    boost::posix_time::ptime m_first_send_time;

    /** the actual system time when the last packet was sent */
    boost::posix_time::ptime m_last_send_time;

    boost::posix_time::ptime m_send_time;

    boost::posix_time::time_duration m_time_since_last_packet;
    boost::posix_time::time_duration m_payload_duration_sum;
    boost::posix_time::time_duration m_last_payload_duration;
    boost::posix_time::time_duration m_total_play_time;

    boost::posix_time::time_duration m_transport_buffer_duration;
  
    CRTPPacket *m_rtp_packet;
    CSync m_syncobj;

    std::list<CStreamConnection*> m_connection_list;

    CMutex m_connection_list_mutex;

    unsigned long m_session_id;

    unsigned long m_stream_id;

    int m_std_client_port;



private:
    void sendToAllClients(CRTPPacket* packet);
};

#endif
