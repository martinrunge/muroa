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
#include "cfloatresampler.h"
#include "csync.h"
#include "cplayer.h"
#include "crtppacket.h"

#include <time.h> 




using namespace std;
using namespace boost::posix_time;
/** C-tor */

CPlayloop::CPlayloop(CPlayer* parent, CPacketRingBuffer* packet_ringbuffer, std::string sound_dev)
 : CThreadSlave(), m_counter(0), m_average_size(32)
{

  m_player = parent;

  int desired_sample_rate = 48000;
  m_frames_to_discard = 0;


  m_packet_ringbuffer = packet_ringbuffer;

  m_ringbuffer = new CRingBuffer(4096);

  m_sample_size = sizeof(short);
  m_num_channels = 2;

  // m_audio_sink = new CAudioIoAlsa();  
  m_audio_sink = initSoundSystem();
  m_audio_sink->open(sound_dev, desired_sample_rate, 2);
  
  int actual_sample_rate = m_audio_sink->getActualSampleRate();
  cerr << "CPlayloop::CPlayloop: open audio sink: try " << desired_sample_rate << " ... succeeded with " << actual_sample_rate << endl;


  m_frames_per_second_pre_resampler = 44100;
  m_frames_per_second_post_resampler = actual_sample_rate;


  m_nr_of_last_frame_decoded = 0;

  // m_resampler = new CResampler(m_ringbuffer, SRC_SINC_BEST_QUALITY, 2);
  m_resampler = static_cast<CResampler*>(new CFloatResampler(m_ringbuffer, high, 2));
  m_resample_factor = (double) m_frames_per_second_post_resampler/m_frames_per_second_pre_resampler; 
  m_correction_factor = 1.0;

  m_num_multi_channel_samples_played = 0;
  m_num_multi_channel_samples_arrived = 0;



  cerr << "Audio sink granularity = " << m_audio_sink->getWriteGranularity() << endl;
  
  m_seqnum = 0;
  
  m_stream_id = 0;
  m_session_id = 0;

  m_start_time = 0;

  int num_shorts = m_audio_sink->getWriteGranularity() / sizeof(short);
  m_silence_buffer = new short[num_shorts];
  short silence_val = 0;

  for(int i=0; i < num_shorts; i++) {
    m_silence_buffer[i] = silence_val;
  }

  m_debug_fd1 = fopen("m_debug_fd1","w");
}

/** D-tor */

CPlayloop::~CPlayloop()
{
  delete[] m_silence_buffer;    

  delete m_start_time;
  delete m_resampler;
  delete m_ringbuffer;
  m_audio_sink->close();
  fclose(m_debug_fd1);
}

/** the main loop for this thread */
void CPlayloop::DoLoop() {
  
  if(m_packet_ringbuffer->getRingbufferSize() == 0) {
     // cerr << "CPlayloop::DoLoop: buffer empty!" << endl;
     usleep(30000);
     return;
  }
  CAudioFrame* frame;

  CRTPPacket* rtp_packet = m_packet_ringbuffer->readPacket();
  //cerr << "packet Buffer size: " << m_packet_ringbuffer->getRingbufferSize() << endl;
  // cerr << "PayloadType " << rtp_packet->payloadType() << " size " << rtp_packet->usedPayloadBufferSize() << endl;

 
  switch( rtp_packet->payloadType() ) 
  {
    case PAYLOAD_SYNC_OBJ:
      m_player->setSyncObj(rtp_packet);
      sync();
        
      delete rtp_packet;
      break;


    case PAYLOAD_PCM:
    case PAYLOAD_MP3:
    case PAYLOAD_VORBIS:
    case PAYLOAD_FLAC:
      if(checkStream(rtp_packet)) {
        frame = new CAudioFrame(rtp_packet);
        playAudio(frame);        
        delete frame;
      }
      delete rtp_packet;
      
      break;

    default:
      cerr << "unknown payload type!" << endl;
      delete rtp_packet;
  }

}


void CPlayloop::playAudio(CAudioFrame *frame) {


  int retval;
  // if(m_num_multi_channel_samples_arrived != frame->firstSampleNr())
  //  cerr << "multi channel samples arrived = " << m_num_multi_channel_samples_arrived << " but rtp timestamp says : " << frame->firstSampleNr() << endl;
  
  // m_num_multi_channel_samples_arrived += frame->dataSize() / (2 * sizeof(short));
  m_num_multi_channel_samples_arrived = frame->firstFrameNr();
  
  m_nr_of_last_frame_decoded = frame->firstFrameNr() + frame->sizeInMultiChannelSamples() - 1;

  // fwrite(frame->dataPtr(), 1, frame->dataSize(), m_debug_fd1);
  int num_single_chan_samples = m_resampler->resampleFrame(frame, m_resample_factor * m_correction_factor);
  

  ///@TODO check timestamp and compare to num of frames in resampler + ringbufer + soundcard. If neccessary, 
  /// call sync() to through away som frames from the ringbuffer, of wait in playing silence samples, to get in sync again.



  int rb_size = m_ringbuffer->size();

  // cerr << num_single_chan_samples * 2 << " bytes written to ringbuffer. size in byte now: " << rb_size << endl;
  int granulated_num_bytes = rb_size - (rb_size % m_audio_sink->getWriteGranularity());
  
  char* playbuffer = m_ringbuffer->read(granulated_num_bytes);

 
  if(m_frames_to_discard > 0) {
    //delete playbuffer;
    adjustFramesToDiscard(granulated_num_bytes / (m_sample_size * m_num_channels));
  }
  
  if(playbuffer != 0 && granulated_num_bytes != 0 && m_frames_to_discard == 0) {
    retval = m_audio_sink->write(playbuffer, granulated_num_bytes);
  
    if(retval == 0 ) {
      cerr << "syncing due to buffer underrun!" << endl;
      sync();
    }
    else {
      adjustResamplingFactor(m_ringbuffer->size());
    }
  }
  delete[] playbuffer;

}

/** called whenever the a resync is neccesarry. For example at the start of e new stream of if the soundcard had an unterrun.   
  Two possibilities:                                                                                                          
  - Time is proceeding and audio data was not played. -> Audio data must be thrown away because to get into sync again        
  - Audio data is missing (e.g. playback underrun). Wait for enough audio data to arrive and start the sound device then.      */ 

int CPlayloop::sync(void) {
 
 
  // this is the difference in time between the clock and the time calculated 
  // from played samples. It sould be zero.
  // sync_diff > 0:
  //   we are late, e.g. due tu a soundcard playback underrun. 
  //   now, there are more samples available than needed.
  //   take the diff in time, calc the diff in frames and through away that amount 
  //   of samples from the ringbuffer.
  // sync_diff < 0:
  //   we are to early. not enough data to playback arrived. calc the amount of 
  //   silence samples to play for waiting, or call nanosleep. Whatever gives the 
  //  better results.
  time_duration sync_diff = getPlaybackDiffFromTime();

  cerr << "CPlayloop::sync: sync_diff = " << sync_diff << endl;

  double diff_in_s = sync_diff.seconds() + 60 * sync_diff.minutes() + 60 * 60 * sync_diff.hours();
  long fractional_secs = sync_diff.fractional_seconds();
  // if the resolution off fractional_seconds() id nano sec, then convert it to micro sec
  if (time_duration::resolution() == boost::date_time::nano) {
    fractional_secs /= 1000;
  }
  diff_in_s += 0.000001 * fractional_secs; 

  double diff_in_frames = diff_in_s * m_frames_per_second_post_resampler;
  long sync_diff_in_frames = lrint(diff_in_frames);
  
  if(sync_diff_in_frames < 0) {
    cerr << "sync: " << sync_diff_in_frames << " too late with playback. trowing away samples." << endl;
    m_frames_to_discard = -sync_diff_in_frames;
  }
  else {
    cerr << "sync: " << sync_diff_in_frames << " too early with playback. waiting while playing silence." << endl;
    m_frames_to_discard = 0;
    sleep(sync_diff);
    // playSilence(sync_diff_in_frames);
  }

  m_session_id = m_player->syncObj()->sessionId();
  m_stream_id = m_player->syncObj()->streamId();

  cerr << "CPlayloop::sync: should be in sync now: sync_diff = " << getPlaybackDiffFromTime() << endl;

  return 0;
}



/*!
    \fn CPlayloop::adjustResamplingFactor()
 */
void CPlayloop::adjustResamplingFactor(int bytes_in_playback_ringbuffer)
{
  // calculation of the drift:
  // measure clock time and time by consumed sampes in every loop run.
  // build an average value every 10 loop runs.

  m_num_multi_channel_samples_played = m_num_multi_channel_samples_arrived - bytes_in_playback_ringbuffer - m_audio_sink->getDelay();   // resampled_frame will be playback rungbuffer 


  ptime now = microsec_clock::local_time();
    
  time_duration play_time_from_clock = now - *(m_player->syncObj()->getPtimePtr());
  time_duration play_time_from_samples = millisec((m_num_multi_channel_samples_played * 10) / 441);  
  time_duration time_diff = play_time_from_clock - play_time_from_samples;

  m_average_time_diff += time_diff;

  m_counter++;
  if(m_counter > m_average_size) {
    
    //    measure the quality of the below usleep calculation
    // ptime now = microsec_clock::local_time();
    // interval = now - (*m_start_time);
    // cerr << "interval: " << interval << endl;

    // m_last_send_time = now;    
    
    m_average_time_diff /= m_average_size;

    int post_delay = m_audio_sink->getDelay(); // number of frames in the playback buffer of the soundcard / sound driver
    int ringbuffer_frames = m_packet_ringbuffer->getRingbufferSize() * 256;   // one ringbuffer frame contains 256 frames

    cerr << "Average time diff (clock - samples) = " << m_average_time_diff <<  " factor used = " << m_resample_factor * m_correction_factor << endl;


    int diff_in_us = m_average_time_diff.fractional_seconds();
    
    // if the resolution off fractional_seconds() id nano sec, then convert it to micro sec
    if (time_duration::resolution() == boost::date_time::nano) {
      diff_in_us /= 1000;
    }
  
    diff_in_us += m_average_time_diff.total_seconds() * 1000000;

    double diff_in_s = 0.000001 * diff_in_us * 0.1;

    m_correction_factor = 1.0 - diff_in_s;  

    // cerr << "total: " << ringbuffer_frames + post_delay << " frames. Alsa(" << post_delay << ")" << endl;
    m_average_time_diff = seconds(0);
    m_counter = 0;
  }
}


int CPlayloop::getDelayInMultiChannelSamples() {
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
    int tmp = m_frames_per_second_post_resampler / 1000;
    latency = millisec(m_audio_sink->getDelay() / tmp);

    return latency;
}


/*!
    \fn CPlayloop::calcResamplerDelay()
 */
time_duration CPlayloop::calcResamplerDelay()
{
    time_duration latency;
    int tmp = m_frames_per_second_pre_resampler / 1000;
    latency = millisec(m_resampler->sizeInMultiChannelSamples() / tmp);

    return latency;
}

/*!
    \fn CPlayloop::calcRingbufferDelay()
 */
time_duration CPlayloop::calcRingbufferDelay()
{
    time_duration latency;
    int tmp = m_frames_per_second_post_resampler / 1000;
    latency = millisec(m_ringbuffer->sizeInMultiChannelSamples() / tmp);

    return latency;
}


/*!
    \fn CPlayloop::playSilence(int num_frames)
 */
void CPlayloop::playSilence(int num_frames)
{
  int granul_in_frames = m_audio_sink->getWriteGranularity() * m_num_channels * sizeof(short);     
  int num_blocks = num_frames / granul_in_frames;
  
  m_frames_to_discard = 0;

  for(int i = 0; i < num_blocks; i++)
    m_audio_sink->write((char*)m_silence_buffer, m_audio_sink->getWriteGranularity()); 
    
}


/*!
    \fn CPlayloop::getPlaybackDiff()
 */
time_duration CPlayloop::getPlaybackDiff()
{
    m_num_multi_channel_samples_played = m_num_multi_channel_samples_arrived 
                                       - m_ringbuffer->sizeInMultiChannelSamples() 
                                       - m_audio_sink->getDelay();   // resampled_frame will be playback rungbuffer 


  ptime now = microsec_clock::local_time();
    
  time_duration play_time_from_clock = now - (*m_player->syncObj()->getPtimePtr());
  time_duration play_time_from_samples = millisec((m_num_multi_channel_samples_played * 10) / 441);  
  time_duration time_diff = play_time_from_clock - play_time_from_samples;

  return time_diff;
}


time_duration CPlayloop::getPlaybackDiffFromTime() {

  long long tmp;

  ptime now = microsec_clock::local_time();

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
  // from played samples. It sould be zero.
  // sync_diff > 0:
  //   we are late, e.g. due tu a soundcard playback underrun. 
  //   now, there are more samples available than needed.
  //   take the diff in time, calc the diff in frames and through away that amount 
  //   of samples from the ringbuffer.
  // sync_diff < 0:
  //   we are to early. not enough data to playback arrived. calc the amount of 
  //   silence samples to play for waiting, or call nanosleep. Whatever gives the 
  //  better results.
  time_duration sync_diff = post_ringbuffer - pre_soundcard;
  
  return sync_diff;

}

/*!
    \fn CPlayloop::sleep(int duration)
 */
int CPlayloop::sleep(time_duration duration)
{
  int retval = 0;


  if( !duration.is_negative() )
  {
    struct timespec ts_to_sleep, ts_remaining;
    ts_to_sleep.tv_sec = duration.total_seconds();
    ts_to_sleep.tv_nsec = duration.fractional_seconds();
    
    //boost::date_time::time_resolutions 
    if (time_duration::resolution() == boost::date_time::micro) {
      ts_to_sleep.tv_nsec *= 1000;
    }
    
    int retval = nanosleep( &ts_to_sleep, &ts_remaining);
    if(retval != 0)
      cerr << "nanosleep returned early due to a signal!" << endl;
    
  }
    
  return retval;  
}

int CPlayloop::adjustFramesToDiscard(int num_frames_discarded) {
  // cerr << "CPlayloop::adjustFramesToDiscard(" << num_frames_discarded << ")" << endl;

  if(m_frames_to_discard < num_frames_discarded) {
    cerr << "CPlayloop::adjustFramesToDiscard: ERROR: m_frames_to_discard < num_frames_discarded. " << endl;
    m_frames_to_discard = 0;
    return -1;  
  }

  m_frames_to_discard -= num_frames_discarded;

  if(m_frames_to_discard < m_ringbuffer->sizeInMultiChannelSamples()) {
    char *discard_ptr = m_ringbuffer->read( m_frames_to_discard * m_num_channels * m_sample_size);
    delete discard_ptr;

    m_frames_to_discard = 0;

  }

  // cerr << "CPlayloop::adjustFramesToDiscard: still " << m_frames_to_discard << " frames left to discard. RB size = " << m_ringbuffer->sizeInMultiChannelSamples() << endl;

  return m_frames_to_discard;
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
  status = stat("/proc/asound/devices", &buffer);
  if(status == 0) {
    ptr = static_cast<IAudioIO*>(new CAudioIoAlsa());
  }
  else {
    ptr = static_cast<IAudioIO*>(new CAudioOSS());
  }
  return ptr;
}



















void CPlayloop::handleSyncObj(CSync* sync_obj) {

  cerr << "got sync obj: ";  
  m_player->syncObj()->print();
  
  if(m_player->syncObj()->streamId() != m_stream_id || m_player->syncObj()->sessionId() != m_session_id) {
    // this is a sync obj for a new stream !!!!
    if(m_start_time != 0)  delete m_start_time;
    m_start_time = new ptime(microsec_clock::local_time());
  }
  
  time_duration sleep_time = (*m_player->syncObj()->getPtimePtr()) - (*m_start_time);
  cerr << "sleep time calculated from sync obj: " << sleep_time << endl;
  
  // sleep_time -= milliseconds(100);

  if( !sleep_time.is_negative() )
  {
    struct timespec ts_to_sleep, ts_remaining;
    ts_to_sleep.tv_sec = sleep_time.total_seconds();
    ts_to_sleep.tv_nsec = sleep_time.fractional_seconds();
    
    //boost::date_time::time_resolutions 
    if (time_duration::resolution() == boost::date_time::micro) {
      ts_to_sleep.tv_nsec *= 1000;
    }
    
    int retval = nanosleep( &ts_to_sleep, &ts_remaining);
    if(retval != 0)
      cerr << "nanosleep returned early due to a signal!" << endl;
    
  }
        
}  





/*!
    \fn CPlayloop::setSync(CSync* sync_obj)
 */
void CPlayloop::setSync(CSync* sync_obj)
{
  cerr << "CPlayloop::setSync" << endl;
  if(m_stream_id != sync_obj->streamId() || m_session_id != sync_obj->sessionId()) {
    // new stream: set m_nr_of_last_frame_decoded to the 
    // frame nr specified in the sync obj (usually 0)
    m_nr_of_last_frame_decoded = sync_obj->frameNr();
  }
  m_session_id = sync_obj->sessionId();
  m_stream_id = sync_obj->streamId();

}
