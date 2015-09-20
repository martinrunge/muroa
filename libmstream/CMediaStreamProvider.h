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

#include "CStreamCtrlConnection.h"

#include "crtppacket.h"

// to be removed soon
#include "csync.h"
#include "cmutex.h"


#include <log4cplus/logger.h>

namespace muroa {

class CMediaStreamProvider {
public:
	CMediaStreamProvider(boost::asio::io_service& io_service, int session_id, int transport_buffer_size_in_ms);
	virtual ~CMediaStreamProvider();

    int open(int audio_bytes_per_second = 2 * 2 * 44100);
    void close();
    void flush();

    int write(char* buffer, int length);
    int sendPacket(char* buffer, int length);

    void addJoinedConnection(CStreamCtrlConnection* conn);
    void removeJoinedConnection(CStreamCtrlConnection* conn);


    CSync* getSyncObj(uint32_t session_id, uint32_t stream_id);

    void stdClientPort(int port);
    int stdClientPort(void);
    void listClients(void);


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

    log4cplus::Logger m_timing_logger;

    CRTPPacket *m_rtp_packet;
    CSync m_syncobj;

    CMutex m_connection_list_mutex;
    std::set<CStreamCtrlConnection*> m_joined_connections;

    unsigned long m_session_id;

    unsigned long m_stream_id;

    int m_std_client_port;
    int m_time_server_port;

    boost::asio::io_service& m_io_service;

    void sendToAllClients(CRTPPacket* packet);
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CMEDIASTREAMPROVIDER_H_ */
