//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Martin Runge <martin.runge@web.de>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include <iostream>

#include "caudiooss.h"

CAudioOSS::CAudioOSS() {
    m_is_open = false;
    m_samplerate = 0;
}


CAudioOSS::~CAudioOSS() {
   if(m_is_open) close();
}

int CAudioOSS::close() {
    int retval;
    retval = ::close(m_snd_dev);
    m_is_open = false;
    return retval; 
}
    
int CAudioOSS::open(std::string device, int samplerate, int channels) {
  m_snd_dev = ::open(device.c_str(), O_WRONLY);
  if(m_snd_dev == -1) {
    perror(device.c_str());
    return -1;
  }
  
  m_snd_fmt = AFMT_S16_LE;
  if(ioctl(m_snd_dev, SNDCTL_DSP_SETFMT, &m_snd_fmt) == -1) {
    perror("Error while setting sound format\n");
    return -2;
  }

  if(m_snd_fmt != AFMT_S16_LE) {
    fprintf(stderr, "Sound format S16_LE not supported.\n");
    return -1;
  }

  m_snd_num_ch = channels;
  if(ioctl(m_snd_dev, SNDCTL_DSP_CHANNELS, &m_snd_num_ch) == -1) {
    perror("Error while setting number of channels\n");
    return -4;
  }

  if(m_snd_num_ch != channels) {
    fprintf(stderr, "Hardware supportes %d channels only.\n", m_snd_num_ch);
    return -8;
  }


  m_snd_rate = samplerate;
  if(ioctl(m_snd_dev, SNDCTL_DSP_SPEED, &m_snd_rate) == -1) {
    perror("Error while setting sampling rate.\n");
    return -16;
  }
  m_samplerate = m_snd_rate;

  if(m_snd_rate != samplerate) {
    fprintf(stderr, "Hardware supportes %d Hz sampling rate only.\n", m_snd_num_ch);
    return -32;
  }
  
  m_is_open = true;
  return 0;

}

int CAudioOSS::read(char* data, int buffersize) {
   if(m_is_open == false) return -1;
   
	return ::read(m_snd_dev, (void*)data, buffersize);
}

int CAudioOSS::write(char* data, int length) {
   if(m_is_open == false) return -1;
	
	return ::write(m_snd_dev, (void*)data, length);

}

int CAudioOSS::getBytesPlayed() {
  count_info num_bytes;
  
  if(ioctl(m_snd_dev, SNDCTL_DSP_GETOPTR, &num_bytes) == -1) {
    perror("Error while getting  numof bytes played\n");
    return -1;
  }
  return num_bytes.bytes;
}

int CAudioOSS::getOutOverflows() {
//  audio_errinfo errinfo;
  
//  if(ioctl(m_snd_dev, SNDCTL_DSP_GETERROR, &errinfo) == -1) {
//    perror("Error while getting error info\n");
//    return -1;
//  }
  return 0; //errinfo.play_underruns;
}


/*!
    \fn CAudioOSS::getDelay();
 */
int CAudioOSS::getDelay()
{
    return -1;    /// @todo implement me
}



int CAudioOSS::getWriteGranularity(void) {
  
  // implement me: fragnment size in OSS
  return 0;
  
}


int CAudioOSS::getActualSampleRate(void) {
  return m_sample_rate;
}