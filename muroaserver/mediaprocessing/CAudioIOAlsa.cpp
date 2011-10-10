/***************************************************************************
 *   Copyright (C) 2004 by Martin Runge                                    *
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
#include "CAudioIOAlsa.h"

#include <iostream>

using namespace std;

CAudioIoAlsa::CAudioIoAlsa()
 : IAudioIO()
{
  m_sample_rate = 0;
  m_open = false;
}


CAudioIoAlsa::~CAudioIoAlsa()
{
}


int CAudioIoAlsa::close() {
  if(m_open == true)
  {
    m_open = false;  
    cerr << "closing audio device" << endl;
    snd_pcm_status_free(m_status_ptr);
    snd_pcm_close(m_playback_handle);
  }
  return 0;
}

int CAudioIoAlsa::open(std::string device, int samplerate, int channels) {

  int err;
  snd_pcm_uframes_t periodsize = 1024;

  if(m_open == true)
    return 0;


  
  if ((err = snd_pcm_open(&m_playback_handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0)
  {
    fprintf(stderr, "cannot open audio device %s (%s)\n", device.c_str(), snd_strerror(err));
    return -1;
  }

  if ((err = snd_pcm_hw_params_malloc(&m_hw_params)) < 0)
  {
    fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
    return -2;
  }

  if ((err = snd_pcm_hw_params_any(m_playback_handle, m_hw_params)) < 0)
  {
    fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
    return -3;
  }

  if ((err = snd_pcm_hw_params_set_access(m_playback_handle, m_hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
    return -4;
  }

  if ((err = snd_pcm_hw_params_set_format(m_playback_handle, m_hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
  {
    fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
    return -5;
  }

  if ((err = snd_pcm_hw_params_set_rate_near(m_playback_handle, m_hw_params, (unsigned*)&samplerate, 0)) < 0)
  {
    fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
    return -6;
  }
  m_sample_rate = samplerate;

  if ((err = snd_pcm_hw_params_set_channels(m_playback_handle, m_hw_params, channels)) < 0)
  {
    fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
    return -7;
  }

  snd_pcm_uframes_t bufsize = periodsize * 16;
  
  cerr << "trying to set buffer size to " << bufsize << " frames, ";
  err = snd_pcm_hw_params_set_buffer_size_near(m_playback_handle, m_hw_params, &bufsize);
  if (err < 0) {
    printf("Unable to set buffer size %li: %s\n", bufsize, snd_strerror(err));
    return err;
  }
  cerr << " successfull with " << bufsize << " frames!" << endl;
  
  cerr << "trying to set period size to " << periodsize << " bytes, ";        
  err = snd_pcm_hw_params_set_period_size_near(m_playback_handle, m_hw_params, &periodsize, 0);
  if (err < 0) {
    printf("Unable to set period size %li: %s\n", periodsize, snd_strerror(err));
    return err;
  }
  cerr << " successfull with " << periodsize << " bytes!" << endl;
  m_write_granularity = periodsize;

  
  if ((err = snd_pcm_hw_params(m_playback_handle, m_hw_params)) < 0)
  {
    fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
    return -8;
  }

  snd_pcm_hw_params_free(m_hw_params);
  if ((err = snd_pcm_prepare(m_playback_handle)) < 0)
  {
    fprintf(stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
    return -9;
  }
  
  
  if ((err = snd_pcm_sw_params_malloc (&m_sw_params)) < 0) {
    fprintf (stderr, "cannot allocate software parameters structure (%s)\n", snd_strerror (err));
    return -10;
  }
  if ((err = snd_pcm_sw_params_current (m_playback_handle, m_sw_params)) < 0) {
    fprintf (stderr, "cannot initialize software parameters structure (%s)\n", snd_strerror (err));
    return -11;
  }
  if ((err = snd_pcm_sw_params_set_avail_min (m_playback_handle, m_sw_params, periodsize)) < 0) {
    fprintf (stderr, "cannot set minimum available count (%s)\n", snd_strerror (err));
    return -12;
  }
  if ((err = snd_pcm_sw_params_set_start_threshold (m_playback_handle, m_sw_params, bufsize / 2)) < 0) {
    fprintf (stderr, "cannot set start mode (%s)\n", snd_strerror (err));
    return -13;
  }
  if ((err = snd_pcm_sw_params (m_playback_handle, m_sw_params)) < 0) {
    fprintf (stderr, "cannot set software parameters (%s)\n", snd_strerror (err));
    return -14;
  }
  
  snd_pcm_sw_params_free(m_sw_params);
 

  if ((err = snd_pcm_status_malloc(&m_status_ptr) < 0)) {
    cerr << "failed to allocate snd_pcm_status_t!" << endl;
    return -15;
  }
  
  m_open = true; 
  return 0;
}

int CAudioIoAlsa::read(char* data, int buffersize){

}

int CAudioIoAlsa::write(char* data, int length) {
  int err;
  int frames_written;  

  frames_written = snd_pcm_writei(m_playback_handle, data, length / 4);

  if( length != frames_written * 4) {
    cerr << "length = " << length << " but only " << frames_written << " frames written." << endl;

    if (frames_written < 0) {
      if(abs(frames_written) == EBADFD)
        cerr << "EBADFD" << endl;
      if(abs(frames_written) == EPIPE)
        cerr << "EPIPE" << endl;
      if(abs(frames_written) == ESTRPIPE)
        cerr << "ESTRPIPE" << endl;
      if(abs(frames_written) == EAGAIN)
        cerr << "EAGAIN" << endl;
        
      fprintf(stderr, "write to audio interface failed (%s) len: %d\n", snd_strerror(frames_written), frames_written);  
      err = snd_pcm_prepare(m_playback_handle);
      if(err != 0) {
        cerr << "snd_pcm_prepare failed: " << snd_strerror(err) << endl;
      }
    }
    return 0;
  }
  return frames_written;

}

int CAudioIoAlsa::getBytesPlayed(){
  return 0;
}

int CAudioIoAlsa::getOutOverflows(){
  return 0;
}




/*!
    \fn CAudioIoAlsa::getDelay()
 */
int CAudioIoAlsa::getDelay()
{
  if(m_open == true)
  {
    snd_pcm_sframes_t delay;
    int retval;

    retval = snd_pcm_delay(m_playback_handle, &delay);
    if(retval != 0) {
      cerr << "CAudioIoAlsa::getDelay: Error." << endl;
      return -1;
    }

    return delay;
  }
  else
    return 0;
}


int CAudioIoAlsa::getWriteGranularity(void) {
  return m_write_granularity;
}


int CAudioIoAlsa::getActualSampleRate(void) {
  return m_sample_rate;
}
