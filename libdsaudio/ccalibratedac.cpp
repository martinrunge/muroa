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
#include "ccalibratedac.h"
#include "caudiooss.h"

#include "time.h"

#include <iostream>

using namespace std;

CCalibrateDAC::CCalibrateDAC() {


  cerr << "CCalibrateDAC::CCalibrateDAC()" << endl;
  m_test_time_in_s = 20;
  m_num_test_samples = 200;

  m_num_channels = 2;
  m_sample_size = sizeof(short);
  m_sample_rate = 48000;

  m_bytes_per_sec = m_sample_rate * m_num_channels * m_sample_size;
  
  m_audio = new CAudioOSS();
  m_testbuffer = new short[m_num_channels * m_num_test_samples];
  
  m_testbuffer_size = m_num_channels * m_num_test_samples * m_sample_size;
  
  
  m_num_cycles = getNumOfCycles();
  
}


CCalibrateDAC::~CCalibrateDAC()
{
  delete [] m_testbuffer;
  delete m_audio;
}

double CCalibrateDAC::measureSampleRate() {
  
  doCalibrate();
  return m_measured_samping_rate;

}


void CCalibrateDAC::doCalibrate() {
  int retval;
  int bytes_played_start, bytes_played_end;
  struct timeval start_time, end_time;
        

    
  retval = m_audio->open("/dev/dsp", m_sample_rate, m_num_channels);
  if(retval != 0)
    return;
    
  // m_audio->write((char*)m_testbuffer, m_testbuffer_size);
  
  // bytes_played_start = m_audio->getBytesPlayed();
  gettimeofday(&start_time, NULL);

  for(int i = 0; i < m_num_cycles; i++) {
    m_audio->write((char*)m_testbuffer, m_testbuffer_size);
  }   

  // bytes_played_end = m_audio->getBytesPlayed();
  
  m_audio->close();
  gettimeofday(&end_time, NULL);

  
  
  // int num_bytes_played = bytes_played_end - bytes_played_start;
  int num_bytes_played = m_bytes_per_sec * m_test_time_in_s;
  long time_in_us = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
  
  cerr << "CCalibrateDAC: played " << num_bytes_played << " in " << time_in_us << " us. ";
  
  double num_multichannel_samples_played = num_bytes_played / (m_num_channels * m_sample_size);
  double time_in_s = (double)time_in_us / 1000000;
  
  m_measured_samping_rate = num_multichannel_samples_played / time_in_s;
  
  cerr << " => " << m_measured_samping_rate << " measured." << endl;
  
}


int CCalibrateDAC::getNumOfCycles() {
  int num;
  
  num = m_test_time_in_s * (m_sample_rate / m_num_test_samples);

  return num;
}