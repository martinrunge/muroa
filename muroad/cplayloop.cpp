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
#include "cplayloop.h"
#include "caudioframe.h"

#include <iostream>
#include "libdsaudio.h"
#include "libsock++.h"
#include "cpacketringbuffer.h"
#include "cringbuffer.h"

#include "CApp.h"
#include "CSettings.h"

#ifndef FIXPOINT
#include "cfloatresampler.h"
#endif

#include "cfixpointresampler.h"
#include "csync.h"
#include "cplayer.h"
#include "cmuroad.h"
#include "crtppacket.h"

#include <time.h> 
#include <math.h>
#include <sys/stat.h>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>



using namespace std;
using namespace boost::posix_time;
using namespace muroa;
using namespace log4cplus;
/** C-tor */

CPlayloop::CPlayloop(CPlayer* parent, CApp *app, CPacketRingBuffer* packet_ringbuffer)
 : CThreadSlave(), m_counter(0), m_average_size(32), m_app(app), m_settings(app->settings()), m_after_sync(false), m_stream_reset_threshold(0.2f)
{

  m_player = parent;
  
  m_timing_logger = Logger::getInstance("timing");

  m_desired_sample_rate = 48000;
  m_frames_to_discard = 0;

  m_periods_to_start = 4;

  m_packet_ringbuffer = packet_ringbuffer;

  m_ringbuffer = new CRingBuffer(2*40960);

  m_sample_size = sizeof(short);
  m_num_channels = 2;

  m_max_idle = m_settings.getProptery("MaxIdle", 100);


  // m_audio_sink = new CAudioIoAlsa();  
  m_audio_sink = initSoundSystem();
  string audio_device = m_settings.getProperty(string("muroa.AudioDevice"), string("hw:0,0"));
  m_audio_sink->open(audio_device, m_desired_sample_rate, m_num_channels);

  int actual_sample_rate = m_audio_sink->getActualSampleRate();
  cerr << "CPlayloop::CPlayloop: open audio sink: try " << m_desired_sample_rate << " ... succeeded with " << actual_sample_rate << endl;
  m_write_granularity = m_audio_sink->getWriteGranularity();

  m_frames_per_second_pre_resampler = 44100;
  m_frames_per_second_post_resampler = actual_sample_rate;


  m_nr_of_last_frame_decoded = -1;

  #ifndef FIXPOINT
  // m_resampler = new CResampler(m_ringbuffer, SRC_SINC_BEST_QUALITY, 2);
  m_resampler = static_cast<CResampler*>(new CFloatResampler(m_ringbuffer, medium, 2));
  #else
  m_resampler = static_cast<CResampler*>(new CFixPointResampler(m_ringbuffer, best, 2));
  #endif

  m_resample_factor = (double) m_frames_per_second_post_resampler/m_frames_per_second_pre_resampler; 
  m_correction_factor = 1.0;

  m_num_multi_channel_samples_played = 0;
  m_num_multi_channel_samples_arrived = 0;


  cerr << "Audio sink granularity = " << m_audio_sink->getWriteGranularity() << endl;
  
  m_restart_duration = milliseconds(500);

  m_seqnum = 0;
  
  m_stream_id = 0;
  m_session_id = 0;

  m_start_time = 0;
  
  m_secs_idle = 0;

  int num_shorts = m_audio_sink->getWriteGranularity() / sizeof(short);
  m_silence_buffer = new short[num_shorts];
  short silence_val = 0;

  for(int i=0; i < num_shorts; i++) {
    m_silence_buffer[i] = silence_val;
  }

  // m_debug_fd1 = fopen("m_debug_fd1","w");
}

/** D-tor */

CPlayloop::~CPlayloop()
{
  delete[] m_silence_buffer;    

  delete m_start_time;
  delete m_resampler;
  delete m_ringbuffer;
  m_audio_sink->close();
  // fclose(m_debug_fd1);
}

CAudioFrame* CPlayloop::getAudioPacket(bool block) {
	  CAudioFrame* frame = 0;

	  do {
		  if(m_packet_ringbuffer->getRingbufferSize() == 0) {
			  if(block) {
				  bool dataAvail = false;
				  while(!dataAvail || m_packet_ringbuffer->getRingbufferSize() == 0) {
					  dataAvail = waitForData();
				  };
			  } else {
				  return 0;
			  }
		  }

		  CRTPPacket* rtp_packet = m_packet_ringbuffer->readPacket();

		  switch( rtp_packet->payloadType() )
		  {
			case PAYLOAD_SYNC_OBJ:
			  m_player->setSyncObj(rtp_packet);
			  delete rtp_packet;
			  break;

			case PAYLOAD_PCM:
			case PAYLOAD_MP3:
			case PAYLOAD_VORBIS:
			case PAYLOAD_FLAC:
			  if(checkStream(rtp_packet)) {
				frame = new CAudioFrame(rtp_packet);
			  }
			  delete rtp_packet;
			  break;

			default:
			  cerr << "unknown payload type!" << endl;
			  delete rtp_packet;
			  break;
		  }
	  }while(frame == 0);
	  return frame;
}

/**
 * waitForData() waits until RTP packets arrive or timeout.
 * returns true if at least one new packet arrived, false otherwise.
 */
bool CPlayloop::waitForData() {

	int retval = m_player->m_traffic_cond.Wait(1);
    if(retval == 0) {
      string audio_device = m_settings.getProperty(string("muroa.AudioDevice"), string("hw:0,0"));

      m_audio_sink->open(audio_device, m_desired_sample_rate, m_num_channels);
      m_write_granularity = m_audio_sink->getWriteGranularity();
      m_player->idleTime(0);
    }
    else {
      if(retval == ETIMEDOUT) {
        m_player->idleTime( m_player->idleTime() + 1);
      }
    }
    if(m_player->idleTime() > m_max_idle) {
      m_audio_sink->close();
    }

    return (retval==0)?true:false;
}

int CPlayloop::addPacket2RingBuffer(bool block) {
	  CAudioFrame* frame = getAudioPacket(block);
	  if(frame) {
		  m_num_multi_channel_samples_arrived = frame->firstFrameNr();
		  m_nr_of_last_frame_decoded = frame->firstFrameNr() + frame->sizeInMultiChannelSamples() - 1;
		  int num_frames = m_resampler->resampleFrame(frame, m_resample_factor * m_correction_factor);
		  delete frame;
		  return num_frames;
	  }
	  else {
		  return 0;
	  }
}


/** the main loop for this thread */
void CPlayloop::DoLoop() {
  CAudioFrame* frame;
  int pb_state = m_audio_sink->state();

  if(pb_state != IAudioIO::E_RUNNING) {
	  int rc = startStream();
	  if(rc) {
		  // stream could not be started, possibly bestartStreamcause not enough packets were available. Try again
		  LOG4CPLUS_WARN(m_timing_logger, "  could not start stream. Possibly not enough data available ");
		  return;
	  }
  }

  // if possibly, read as many packets out of the packet ringbuffer as neccessary to fill soundcard ringbuffer
  // completely, but do not block if there are not enough packets available.
  int frames_writable = m_audio_sink->getSpace();
  int num_frames;
  while( m_ringbuffer->sizeInFrames() < m_write_granularity) {
	  num_frames = addPacket2RingBuffer(false);
	  // LOG4CPLUS_DEBUG(m_timing_logger, "  ringbuffer was low (" << m_ringbuffer->sizeInFrames() << "), added " << num_frames << " frames.");
  }

  char* playbuffer = m_ringbuffer->readFrames(m_write_granularity);

  int retval = m_audio_sink->write(playbuffer, m_write_granularity * m_num_channels * m_sample_size);
  delete[] playbuffer;
  if(retval == 0 ) {
      LOG4CPLUS_WARN(m_timing_logger, "*** soundcard buffer underrun ***");
	  return;
  }
  else {
	  // LOG4CPLUS_DEBUG(m_timing_logger, "  wrote chunk of " << retval << " frames. Space for: " <<  m_audio_sink->getSpace() << " frames left. state: " << m_audio_sink->state());
  }
  adjustResamplingFactor();
}


int CPlayloop::startStream() {

	// first: fill soundcard buffer with as many data as possible
	int rc = 1;
	int num_frames_writable = m_audio_sink->getSpace();
	m_correction_factor = 1.0f;
	LOG4CPLUS_INFO(m_timing_logger, "");
	LOG4CPLUS_INFO(m_timing_logger, "startStream: m_resample_factor: " <<  m_resample_factor * m_correction_factor);

	// throw away frames that have a presentation time stamp before a reachable start time (e.g. in the past)
	// (now + what it takes until m_audio_sink->start() gets called at the end of this method)
	ptime first_frames_pts = discardPastPTSFrames();

	while(m_ringbuffer->sizeInFrames() < m_write_granularity * m_periods_to_start) {
		addPacket2RingBuffer(true);
	}

	LOG4CPLUS_INFO(m_timing_logger, "step 2: filling soundcard buffer with: " <<  m_write_granularity * m_periods_to_start << " frames (writable: " <<  m_audio_sink->getSpace() << ", state: " << m_audio_sink->state() << ")");
	assert(m_ringbuffer->sizeInFrames() >= m_write_granularity * m_periods_to_start);
	for( int i = 0; i < m_periods_to_start; i++)
	{
		char* playbuffer = m_ringbuffer->readFrames(m_write_granularity);
		// attention, this __should__ never block!!!
		// LOG4CPLUS_DEBUG(m_timing_logger, "  Frames writable now: " <<  m_audio_sink->getSpace() << " state: " << m_audio_sink->state());
		int frame_size = m_num_channels * m_sample_size;
		int written = m_audio_sink->write(playbuffer, m_write_granularity * frame_size);
		// LOG4CPLUS_DEBUG(m_timing_logger, "    Frames written: " <<  written);
		delete [] playbuffer;
	}
	LOG4CPLUS_DEBUG(m_timing_logger, "   ... done. Frames writable now: " <<  m_audio_sink->getSpace() << " state: " << m_audio_sink->state());

	// second: wait for starting time
	waitForStartPTS();
	LOG4CPLUS_INFO(m_timing_logger, "  Frames writable now: " <<  m_audio_sink->getSpace() << " state: " << m_audio_sink->state());
    LOG4CPLUS_INFO(m_timing_logger, "start stream");

    // third: start playback
	rc = m_audio_sink->start();
	m_last_start_stream_error = getPlaybackDiffFromTime();

	LOG4CPLUS_INFO(m_timing_logger, "stream started: " << m_last_start_stream_error << " (new method: " << getCurrentPTSDeviation() << ")");
	return rc;
}


/**
 * All audio frames with PTS in the past may not be played any more. Discard them.
 */
ptime CPlayloop::discardPastPTSFrames() {
	  while(m_nr_of_last_frame_decoded == -1 || (!m_player->syncObj()->isValid())) {
		  addPacket2RingBuffer(true);
	  }
	  ptime* pts = m_player->syncObj()->getPtimePtr();
	  uint32_t pts_frame_nr = m_player->syncObj()->frameNr();
	  LOG4CPLUS_DEBUG(m_timing_logger, "step 1: discard frames with PTS in the past: reference pts for frame " << pts_frame_nr << ": " << *pts);

	  // time and frame diff between last sync and stream state just before the resampler
	  int64_t pre_resampler_frame_diff = m_nr_of_last_frame_decoded - pts_frame_nr;
	  int64_t tmp = pre_resampler_frame_diff * 1000; // to get milliseconds after division next line
	  time_duration pre_resampler_diff = milliseconds( tmp / m_frames_per_second_pre_resampler);
	  ptime pre_resampler_pts = *pts + pre_resampler_diff;

	  LOG4CPLUS_DEBUG(m_timing_logger, "   pre_resampler_frame_diff: " << pre_resampler_frame_diff << " (m_nr_of_last_frame_decoded=" << m_nr_of_last_frame_decoded << ")");
	  LOG4CPLUS_DEBUG(m_timing_logger, "   (reference pts -> last frame decoded ) pre_resampler_diff: " << pre_resampler_diff << " (" << pre_resampler_frame_diff << " frames)");

	  int64_t frames_post_resampler = m_ringbuffer->sizeInFrames();
	  LOG4CPLUS_ERROR(m_timing_logger, "   frames post resampler: " << frames_post_resampler);

	  tmp = 1000000 * frames_post_resampler;
	  time_duration post_resampler_diff = microseconds( tmp / m_frames_per_second_post_resampler);
	  // start_frame_pts: presentation timestamp for the first frame in soundcard's buffer waiting to be started
	  LOG4CPLUS_DEBUG(m_timing_logger, "   post_resampler_diff : " << post_resampler_diff);

	  ptime first_frames_pts = pre_resampler_pts - post_resampler_diff;

	  ptime now = microsec_clock::universal_time();
	  time_duration restart_diff = now - first_frames_pts;
	  int64_t frames_to_discard = restart_diff.total_milliseconds() * ( m_frames_per_second_post_resampler / 1000);

	  LOG4CPLUS_DEBUG(m_timing_logger, "   first frame's pts: " << first_frames_pts << " discarding " << frames_to_discard << " frames");

	  uint32_t frames_discarded = 0;
	  while(frames_to_discard > 0) {
		  while( m_ringbuffer->sizeInFrames() < m_write_granularity) {
			  int num_frames = addPacket2RingBuffer(false);
			  if(num_frames == 0) {
				  LOG4CPLUS_ERROR(m_timing_logger, "   frames should be discarded to get in sync, but there were none in the packetbuffer.");
			  }
		  }

		  int num = ( frames_to_discard > m_write_granularity) ? m_write_granularity : frames_to_discard;
		  char* playbuffer = m_ringbuffer->readFrames(num);
		  delete [] playbuffer;
		  frames_to_discard -= num;
		  frames_discarded += num;
	  }

	  if(frames_discarded > 0) {
		  tmp = 1000000 * frames_discarded;
		  time_duration discard_diff = microseconds( tmp / m_frames_per_second_post_resampler);
		  first_frames_pts += discard_diff;
	  }

	  return first_frames_pts;
}

/** called by startStream() whenever the a waitForStartPTS is necessary to start or restart a stream
 *  Add time probably needed for waitForStartPTS to current time and calculate the frame number to start with.  */
void CPlayloop::waitForStartPTS() {
	  LOG4CPLUS_DEBUG(m_timing_logger, "step 3: wait for start pts ..." );

	  ptime* pts = m_player->syncObj()->getPtimePtr();
	  uint32_t pts_frame_nr = m_player->syncObj()->frameNr();
	  LOG4CPLUS_DEBUG(m_timing_logger, "   pts for frame " << pts_frame_nr << ": " << *pts);

	  // time and frame diff between last sync and stream state just before the resampler
	  uint64_t pre_resampler_frame_diff = m_nr_of_last_frame_decoded - pts_frame_nr;
	  int64_t tmp = pre_resampler_frame_diff * 1000000; // to get microseconds after division next line
	  time_duration pre_resampler_diff = microseconds( tmp / m_frames_per_second_pre_resampler);
	  ptime pre_resampler_pts = *pts + pre_resampler_diff;

	  LOG4CPLUS_DEBUG(m_timing_logger, "   pre_resampler_frame_diff: " << pre_resampler_frame_diff << " (m_last_frame_decoded=" << m_nr_of_last_frame_decoded << ")");
	  LOG4CPLUS_DEBUG(m_timing_logger, "   m_ringbuffer->sizeInFrames():" << m_ringbuffer->sizeInFrames() << "  m_periods_to_start: " << m_periods_to_start << " m_write_granularity: " << m_write_granularity );

	  int64_t frames_post_resampler = m_ringbuffer->sizeInFrames() + m_periods_to_start * m_write_granularity;
	  LOG4CPLUS_ERROR(m_timing_logger, "   frames post resampler: " << frames_post_resampler);

	  tmp = 1000000 * frames_post_resampler;
	  LOG4CPLUS_ERROR(m_timing_logger, "   post_resampler_diff in microsec: " << tmp / m_frames_per_second_post_resampler);

	  time_duration post_resampler_diff = microseconds( tmp / m_frames_per_second_post_resampler);
	  // start_frame_pts: presentation timestamp for the first frame in soundcard's buffer waiting to be started
	  ptime start_frame_pts = pre_resampler_pts - post_resampler_diff;

	  LOG4CPLUS_DEBUG(m_timing_logger, "   pre_resampler_pts: " << pre_resampler_pts << "  post_resampler_diff: " << post_resampler_diff);
	  LOG4CPLUS_ERROR(m_timing_logger, "   about to restart: restart_frame's pts: " << start_frame_pts);

	  int rc = sleepuntil(start_frame_pts);
	  if(rc) {
		  LOG4CPLUS_DEBUG(m_timing_logger, "   CPlayloop::sleepuntil() retuned error" );
	  }
	  else {
		  LOG4CPLUS_DEBUG(m_timing_logger, "   should be in sync now." );
	  }
	  m_after_sync = true;

	  return;
}


/*!
    \fn CPlayloop::adjustResamplingFactor()
 */
void CPlayloop::adjustResamplingFactor()
{


  m_average_time_diff += getCurrentPTSDeviation();
  m_counter++;
  if(m_counter >= m_average_size) {
    m_average_time_diff /= m_counter;

    int diff_in_us = m_average_time_diff.total_microseconds();
    double diff_in_s = 0.000001 * diff_in_us;

    m_correction_factor = 1.0 - diff_in_s * 0.1;

    cerr << "Average time diff (clock - samples) = " << m_average_time_diff <<  " factor used = " << m_resample_factor * m_correction_factor << " RB size: " << m_ringbuffer->sizeInFrames() << endl;

    m_average_time_diff = seconds(0);
    m_counter = 0;

    // if resampling factor would speed up or slow down music too much, prefer a stream reset to get in sync again.
    float up_th = 1.0f + m_stream_reset_threshold;
    float low_th = 1.0f - m_stream_reset_threshold;

    if(m_correction_factor < low_th || m_correction_factor > up_th ) {
  	  LOG4CPLUS_WARN( m_timing_logger, " correction factor '"<< m_correction_factor <<"' is outside desired range of [" << low_th << "," << up_th<< "]");
  	  resetStream(m_session_id, m_stream_id);
  	  startStream();
    }
  }
}




int CPlayloop::getDelayInFrames() {
  int delay;

  delay = m_resampler->sizeInMultiChannelSamples();
  delay += m_ringbuffer->sizeInMultiChannelSamples();
  delay += m_audio_sink->getDelay();  

  return delay;
};

/*!
    \fn CPlayloop::calcSoundCardDelay()
 */
time_duration CPlayloop::calcSoundCardDelay()
{
    time_duration latency;
    int64_t tmp = m_audio_sink->getDelay() * 1000;
    latency = millisec(tmp / m_frames_per_second_post_resampler);
    return latency;
}


/*!
    \fn CPlayloop::calcResamplerDelay()
 */
time_duration CPlayloop::calcResamplerDelay()
{
    time_duration latency;
    int64_t tmp = m_resampler->sizeInMultiChannelSamples() * 1000;
    latency = millisec( tmp / m_frames_per_second_pre_resampler);
    return latency;
}

/*!
    \fn CPlayloop::calcRingbufferDelay()
 */
time_duration CPlayloop::calcRingbufferDelay()
{
    time_duration latency;
    int64_t tmp = m_ringbuffer->sizeInMultiChannelSamples() * 1000;
    latency = millisec(tmp / m_frames_per_second_post_resampler);
    return latency;
}

time_duration CPlayloop::getCurrentPTSDeviation() {
	ptime pre_resampler_pts = getPreResamplerPTS();

	time_duration rb_duration = calcRingbufferDelay();
	time_duration resampler_duration = calcResamplerDelay();
	time_duration sc_duration = calcSoundCardDelay();
	// current_pts: the frame currently played should be played at this time to be in sync.
	ptime current_pts = pre_resampler_pts - resampler_duration - rb_duration - sc_duration;

	ptime now = microsec_clock::universal_time();
	time_duration deviation = now - current_pts;
	return deviation;
}


int CPlayloop::sleepuntil(boost::posix_time::ptime wakeup_time) {
	struct timespec wakeup;
	struct timespec now;

	ptime ptime_now = microsec_clock::universal_time();
	clock_gettime(CLOCK_MONOTONIC, &now);

	time_duration diff = wakeup_time - ptime_now;

	int64_t diff_nsec = diff.total_nanoseconds();
	int64_t diff_sec = diff_nsec / 1000000000;
	diff_nsec -= diff_sec * 1000000000;

	wakeup.tv_sec = now.tv_sec + diff_sec;
	wakeup.tv_nsec = now.tv_nsec + diff_nsec;
	if(wakeup.tv_nsec > 1000000000) {
		int64_t tmp_sec = wakeup.tv_nsec / 1000000000;
		wakeup.tv_sec += tmp_sec;
		wakeup.tv_nsec  -= tmp_sec * 1000000000;
	}

	LOG4CPLUS_DEBUG(m_timing_logger, "    sleepuntil( " << wakeup_time << "):");
    LOG4CPLUS_DEBUG(m_timing_logger, "       now:   tv_sec: " << now.tv_sec << " tv_nsec:" << now.tv_nsec );
    LOG4CPLUS_DEBUG(m_timing_logger, "       wakeup tv_sec: " << wakeup.tv_sec << " tv_nsec:" << wakeup.tv_nsec );


	int rc;
	do {
		rc = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeup, NULL);
	} while(rc == EINTR);

}



/**
    CPlayloop::checkStream(CRTPPacket* packet)

    if the packet belongs to the actual stream, return true. Otherwise request a sync object for that stream and return false.
 */
bool CPlayloop::checkStream(CRTPPacket* packet)
{
    uint32_t tmp_session_id = packet->sessionID();
    uint32_t tmp_stream_id = packet->streamID();

    if(tmp_session_id != m_session_id || tmp_stream_id != m_stream_id ) {
      // this packet does not belong to the actual stream
      cerr << "Got RTP packet of different stream (" << tmp_session_id << "/" << tmp_stream_id
           << "). Self (" << m_session_id << "/" << m_stream_id << "). " << endl; 
      m_player->requestSync(tmp_session_id, tmp_stream_id);
      return false;  
    }
    else
      return true;
}


/*!
    \fn CPlayer::initSoundSystem()
 */
IAudioIO* CPlayloop::initSoundSystem()
{
  struct stat buffer;
  int         status;

  IAudioIO* ptr;

  // check if alsa is available:  
  status = ::stat("/proc/asound/devices", &buffer);
  if(status == 0) {
    ptr = static_cast<IAudioIO*>(new CAudioIoAlsa());
  }
  else {
    ptr = static_cast<IAudioIO*>(new CAudioOSS());
  }
  return ptr;
}



/*!
    \fn CPlayloop::setSync(CSync* sync_obj)
 */
void CPlayloop::setSync(CSync* sync_obj)
{
  LOG4CPLUS_INFO(m_timing_logger, "CPlayloop::setSync " << *sync_obj);
  if(m_stream_id != sync_obj->streamId() || m_session_id != sync_obj->sessionId()) {
    // new stream: set m_nr_of_last_frame_decoded to the 
    // frame nr specified in the sync obj (usually 0)
    m_nr_of_last_frame_decoded = sync_obj->frameNr();
  }
  m_session_id = sync_obj->sessionId();
  m_stream_id = sync_obj->streamId();
}

void CPlayloop::resetStream(uint32_t oldSessionID, uint32_t oldStreamId) {
	m_audio_sink->stop();
}

/*** get the presentation time stamp of the audio frame just before the resampler
 * based on the reference time stamp of the current sync object.
 */
ptime CPlayloop::getPreResamplerPTS() {
	ptime* pts = m_player->syncObj()->getPtimePtr();
	uint32_t pts_frame_nr = m_player->syncObj()->frameNr();

	// time and frame diff between last sync and stream state just before the resampler
	int64_t pre_resampler_frame_diff = m_nr_of_last_frame_decoded - pts_frame_nr;
	int64_t tmp = pre_resampler_frame_diff * 1000; // to get milliseconds after division next line
	time_duration pre_resampler_diff = milliseconds( tmp / m_frames_per_second_pre_resampler);
	ptime pre_resampler_pts = *pts + pre_resampler_diff;

	return pre_resampler_pts;
}












time_duration CPlayloop::getPlaybackDiffFromTime() {

  int64_t tmp;

  ptime now = microsec_clock::universal_time();

  ptime pre_soundcard = now - calcSoundCardDelay();

  // from last sync object -> calc time for m_timestamp_of_last_sample
  // sub delay of the buffers

  if(m_nr_of_last_frame_decoded < m_player->syncObj()->frameNr()) {
    cerr << "CPlayloop::sync: ERROR: m_nr_of_last_frame_decoded < m_sync_obj->frameNr(). Possibly missed a sync object." << endl;
  }
  assert(m_nr_of_last_frame_decoded >= m_player->syncObj()->frameNr());


  long long diff_in_frames = m_nr_of_last_frame_decoded - m_player->syncObj()->frameNr();
  ptime synctime(*m_player->syncObj()->getPtimePtr());

  tmp = m_frames_per_second_pre_resampler / 1000;
  ptime post_packet_ringbuffer = synctime + millisec(diff_in_frames / tmp);

  ptime post_ringbuffer = post_packet_ringbuffer
                        + calcResamplerDelay()
                        + calcRingbufferDelay();


  // this is the difference in time between the clock and the time calculated
  // from played samples. It should be zero.
  // sync_diff > 0:
  //   we are late, e.g. due to a soundcard playback underrun.
  //   now, there are more samples available than needed.
  //   take the diff in time, calc the diff in frames and through away that amount
  //   of samples from the ringbuffer.
  // sync_diff < 0:
  //   we are to early. Not enough data to playback arrived. Calc the amount of
  //   silence samples to play for waiting, or call nanosleep. Whatever gives the
  //  better results.
  time_duration sync_diff = post_ringbuffer - pre_soundcard;

  return sync_diff;

}


/*!
    \fn CPlayloop::adjustResamplingFactor()
 */
void CPlayloop::adjustResamplingFactor(int multichannel_samples_in_playback_ringbuffer)
{
  // calculation of the drift:
  // measure clock time and time by consumed samples in every loop run.
  // build an average value every 10 loop runs.
  m_num_multi_channel_samples_played = m_num_multi_channel_samples_arrived - multichannel_samples_in_playback_ringbuffer - m_audio_sink->getDelay();   // resampled_frame will be playback ringbuffer

  ptime now = microsec_clock::universal_time();

  time_duration play_time_from_clock = now - *(m_player->syncObj()->getPtimePtr());
  time_duration play_time_from_samples = millisec((m_num_multi_channel_samples_played * 10) / 441);
  time_duration time_diff = play_time_from_clock - play_time_from_samples;

  m_average_time_diff += time_diff;
  m_counter++;
  if(m_counter >= m_average_size) {

    //    measure the quality of the below usleep calculation
    // ptime now = microsec_clock::universal_time();
    // interval = now - (*m_start_time);
    // cerr << "interval: " << interval << endl;

    // m_last_send_time = now;

    m_average_time_diff /= m_counter;

    int post_delay = m_audio_sink->getDelay(); // number of frames in the playback buffer of the soundcard / sound driver
    int ringbuffer_frames = m_packet_ringbuffer->getRingbufferSize() * 256;   // one ringbuffer frame contains 256 frames

    cerr << "Average time diff (clock - samples) = " << m_average_time_diff <<  " factor used = " << m_resample_factor * m_correction_factor << " RB size: " << ringbuffer_frames << endl;

    int diff_in_us = m_average_time_diff.fractional_seconds();

    // if the resolution off fractional_seconds() is nano sec, then convert it to micro sec
    if (time_duration::resolution() == boost::date_time::nano) {
      diff_in_us /= 1000;
    }

    diff_in_us += m_average_time_diff.total_seconds() * 1000000;

    double diff_in_s = 0.000001 * diff_in_us * 0.1;

    m_correction_factor = 1.0 - diff_in_s;

    // cerr << "total: " << ringbuffer_frames + post_delay << " frames. Alsa(" << post_delay << ")" << endl;
    m_average_time_diff = seconds(0);
    m_counter = 0;

    // if resampling factor would speed up or slow down music too much, prefer a stream reset to get in sync again.
    float up_th = 1.0f + m_stream_reset_threshold;
    float low_th = 1.0f - m_stream_reset_threshold;

    if(m_correction_factor < low_th || m_correction_factor > up_th ) {
  	  LOG4CPLUS_WARN( m_timing_logger, " correction factor '"<< m_correction_factor <<"' is outside desired range of [" << low_th << "," << up_th<< "]");
  	  //resetStream(m_session_id, m_stream_id);
  	  //startStream();
    }

  }

}
