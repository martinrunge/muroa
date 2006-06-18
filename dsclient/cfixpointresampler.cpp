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
#include <iostream>
#include <assert.h>

#include "cfixpointresampler.h"
#include "caudioframe.h"
#include "cringbuffer.h"



using namespace std;

CFixPointResampler::CFixPointResampler(CRingBuffer *ringbuffer, enum converter_quality quality, int num_channels):
  CResampler(ringbuffer, quality, num_channels)
{
  int error;
  
  m_ringbuffer = ringbuffer;

  m_num_channels = num_channels;
  m_size_of_float_multichannel_sample = m_num_channels * sizeof(float);
 
  m_size_of_input_singlechannel_sample = sizeof(short);
  m_size_of_input_multichannel_sample = m_size_of_input_singlechannel_sample * m_num_channels;

  switch (quality) {
    case poor:
      m_converter = new CFixPointConverter(false, true, false);
      break;

    case medium:
      m_converter = new CFixPointConverter(false, false, true);
      break;

    case high:
    case best:
      m_converter = new CFixPointConverter(true, false, true);
      break;

      
    default:
      m_converter = new CFixPointConverter(false, true, false);
      cerr << "error, unknown resampling quality. Using poor." << endl; 
  }


  
  m_in_buffer_size = 2 * 1024;
  m_out_buffer_size = 3 * 1024;
  
  m_in_buffer = new short[m_in_buffer_size];
  m_out_buffer = new short[m_out_buffer_size];
  
 
  m_in_offset = 0;
  m_out_offset = 0;

  /*m_debug_fd1 = fopen("resampler_output.raw", "w");
  m_data_ptr = new char[4096];*/

  // m_resampled_frame = new CAudioFrame(PAYLOAD_PCM, 3 * 1024);

}


CFixPointResampler::~CFixPointResampler()
{
  // delete m_resampled_frame;

  /*fclose(m_debug_fd1);
  delete [] m_data_ptr;*/

  delete[] m_in_buffer;


  delete[] m_out_buffer;

  delete m_converter;

}




/*!
    \fn CFixPointResampler::resampleFrame(CAudioFrame* in_frame)
 */
int CFixPointResampler::resampleFrame(CAudioFrame* in_frame, double factor)
{
  int res;
  // src_set_ratio (m_src_state, factor);


  appendFrameToImputBuffer(in_frame);


  
  int num_frames = copyResampledFramesToRingbuffer();

  return num_frames;;
}


int CFixPointResampler::reset() {

  int retval;

  m_converter->initialize(1.0, m_num_channels, 1.0);
  
  m_in_offset = 0;
  m_out_offset = 0;
 
  return retval;
}


void CFixPointResampler::appendFrameToImputBuffer(CAudioFrame* in_frame) {


}




int CFixPointResampler::copyResampledFramesToRingbuffer() 
{
  int num_single_channel_samples;

  return num_single_channel_samples;
}


int CFixPointResampler::sizeInMultiChannelSamples() {
  return 0;
}

