/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge                                    *
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
#ifndef CPLAYLOOP_H
#define CPLAYLOOP_H

/**
  @author Martin Runge
*/


#include <cthreadslave.h>
#include "caudioioalsa.h"
#include "caudiooss.h"
#include "cmutex.h"

#include "CSettings.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_duration.hpp>

#include <log4cplus/logger.h>

#include <cstdint>
#include <list>
#include <string>

namespace muroa
{
  class CApp;
}

class CAudioFrame;
class CSync;
class CPacketRingBuffer;
class CRingBuffer;
class CResampler;
class CPlayer;
class Cmuroad;
class CRTPPacket;

using namespace boost::posix_time;

class CPlayloop : public CThreadSlave
{
public:
	CPlayloop(CPlayer* parent, CPacketRingBuffer* packet_ringbuffer );
    ~CPlayloop();

    void DoLoop();
    void setSync(CSync* sync_obj);
    void resetStream(uint32_t oldSessionID, uint32_t oldStreamId);

private:

    double m_resample_factor;
    double m_correction_factor;

    /// write granularity in frames
    int m_write_granularity;

    uint32_t m_stream_id, m_session_id;

    int m_seqnum;
    int m_counter;

    long m_num_frames_arrived;

    boost::posix_time::time_duration m_average_time_diff;
    int m_average_size;

    CAudioFrame* getAudioPacket(bool block);
    bool waitForData();
    int addPacket2RingBuffer(bool block);
    int startStream();
    boost::posix_time::ptime discardPastPTSFrames();
    void waitForStartPTS();
    int sleepuntil(boost::posix_time::ptime wakeup_time);

    void adjustResamplingFactor();

    boost::posix_time::ptime getPreResamplerPTS();

    boost::posix_time::time_duration calcSoundCardDelay();
    boost::posix_time::time_duration calcResamplerDelay();
    boost::posix_time::time_duration calcRingbufferDelay();
    boost::posix_time::time_duration getCurrentPTSDeviation();
    
    boost::posix_time::time_duration m_last_start_stream_error;
    boost::posix_time::time_duration m_max_stream_reset_duration;

    // nr of last audio frame that was not yet resampled. To get the presentation timestamp of this frame,
    // it is save to simply interpolate:
    // pts = syncObj->pts() + (m_last_frame_pre_resampler - syncObj->frameNr() ) * stream_frame_rate
    int64_t m_nr_of_last_frame_decoded;

    int m_desired_sample_rate;
    int m_num_channels;
    int m_sample_size;
    int m_frames_per_second_pre_resampler;
    int m_frames_per_second_post_resampler;

    int m_periods_to_start;
    float m_stream_reset_threshold;

    FILE* m_debug_fd1;
    
    CPlayer* m_player;
    
    CPacketRingBuffer *m_packet_ringbuffer;
    CRingBuffer *m_ringbuffer;
    CResampler* m_resampler;

    //CAudioIoAlsa *m_audio_sink;
    IAudioIO *m_audio_sink;
    IAudioIO* initSoundSystem();

    int m_secs_idle;
    int m_max_idle;

    log4cplus::Logger m_timing_logger;

  private:
    bool checkStream(CRTPPacket* packet);
    bool m_after_sync;
};

#endif
