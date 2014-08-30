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
#include "cposixcond.h"
#include "IRenderCmds.h"
// #include "cmds/CmdStreamReset.h"

#include <boost/asio.hpp>


namespace muroa
{
  class CApp;
  class CSettings;
  class CDnsSdAvahi;
  class CmdStreamReset;
}

class CRecvloop;
class CPlayloop;
class CRTPPacket;
class CPThread;
class CPacketRingBuffer;


class CPlayer : public muroa::IRenderCmds {
public:
    CPlayer(muroa::CApp *app, boost::asio::io_service& io_service);

    ~CPlayer();
    void start();
    void stop();
    void sendRTPPacket(CRTPPacket* packet);

    void onResetStream(const muroa::CmdStreamReset& cmd_rst);
	void onJoinMulticastGroup() {};
	void onLeaveMutlicastGroup() {};


    inline CSync* syncObj() {return &m_sync_obj; };
    void setSyncObj(CRTPPacket* rtp_packet);
    // void sync();

    inline int syncRequestedForStreamID(void) { 
      return m_sync_requested_for_stream_id;
    };

    void requestSync(int session_id, int stream_id);
    void setRequestedSyncObj(CRTPPacket* rtp_packet);

    void idleTime ( int theValue )
    {
        m_idle_time = theValue;
    }

    int idleTime() const
    {
        return m_idle_time;
    }
    

    CPosixCond m_traffic_cond;

private:
    muroa::CDnsSdAvahi *m_dnssd;

    CPacketRingBuffer * m_packet_ringbuffer;
  
    CPlayloop *m_playloop;
    CRecvloop *m_recvloop;

    CPThread *m_recvloop_thread;
    CPThread *m_playloop_thread;

    CSync m_sync_obj;

    int m_sync_requested_for_stream_id;
    boost::posix_time::ptime m_sync_requested_at;
    int m_idle_time;
    
    // Cmuroad* m_config;
    muroa::CApp* m_app;
    muroa::CSettings& m_settings;

    boost::asio::io_service& m_io_service;
};

#endif
