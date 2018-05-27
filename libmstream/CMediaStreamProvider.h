/*
 * CMediaStreamProvider.h
 *
 *  Created on: 20.09.2015
 *      Author: martin
 */

#ifndef LIBMSTREAM_CMEDIASTREAMPROVIDER_H_
#define LIBMSTREAM_CMEDIASTREAMPROVIDER_H_

#include <iostream>
#include <set>
#include <list>
#include <string>

#include <sys/time.h>

#include "CTimeServiceCtrl.h"
#include "CStreamCtrlConnection.h"
#include "CUDPSocket.h"
#include "crtppacket.h"
#include <CStreamFmt.h>

#include "CRenderClientDesc.h"

// to be removed soon
#include "csync.h"
#include "cmutex.h"


#include <log4cplus/logger.h>

namespace muroa {

class CMediaStreamProvider {
public:
	CMediaStreamProvider(boost::asio::io_service& io_service, int session_id, int transport_buffer_size_in_ms);
	virtual ~CMediaStreamProvider();

    int open(muroa::CStreamFmt sfmt);
    int open(int num_channels = 2, int sample_rate = 44100, enum AVSampleFormat sample_format = AV_SAMPLE_FMT_S16);
    void close();
    void flush();
    bool isOpen() { return m_is_open; };
    CStreamFmt getStreamFmt();

    int write(char* buffer, int length);
    int sendPacket(char* buffer, int length);

    void addJoinedConnection(CStreamCtrlConnection* conn);
    int removeJoinedConnection(CStreamCtrlConnection* conn);
    CStreamCtrlConnection* getCtrlConnection(std::string serviceName);

    CSync* getSyncObj(uint32_t session_id, uint32_t stream_id);

    void stdClientPort(int port);
    int stdClientPort(void);
    void listClients(void);

    std::vector<std::string> getJoinedRenderClients();


private:
    unsigned long m_num;
    int m_payload_size;

    int m_num_clients;

    int m_audio_bytes_per_second;
    int m_audio_bytes_per_frame;

    long m_frames_in_sync_period;

    bool m_is_open;
    CStreamFmt m_streamFmt;

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

    log4cplus::Logger m_timing_logger;

    CRTPPacket *m_rtp_packet;

    muroa::CTimeServiceCtrl m_time_service_ctrl;

    muroa::evSyncStream m_sync_info;
    uint32_t m_ssrc;

    CMutex m_connection_list_mutex;
    std::set<CStreamCtrlConnection*> m_joined_connections;

    muroa::CUDPSocket m_mcast_sock;
    bool m_use_mcast;
    bool m_builtin_time_service_used;

    unsigned long m_session_id;

    unsigned long m_stream_id;

    int m_std_client_port;
    int m_time_server_port;

    boost::asio::io_service& m_io_service;

    void sendToAllClients(CRTPPacket* packet);
    void sendToAllClients(const CmdStreamBase* cmd);
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CMEDIASTREAMPROVIDER_H_ */
