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

#include "cfloatresampler.h"
#include "caudioframe.h"
#include "cringbuffer.h"



using namespace std;

CFloatResampler::CFloatResampler(CRingBuffer *ringbuffer, enum converter_quality quality, int num_channels):
  CResampler(ringbuffer, quality, num_channels)
{
  int error;
  int converter_type;

  m_ringbuffer = ringbuffer;

  m_num_channels = num_channels;
  m_size_of_float_multichannel_sample = m_num_channels * sizeof(float);
 
  m_size_of_input_singlechannel_sample = sizeof(short);
  m_size_of_input_multichannel_sample = m_size_of_input_singlechannel_sample * m_num_channels;

  switch (quality) {
    case poor:
      converter_type = SRC_LINEAR;
      break;

    case medium:
      converter_type = SRC_SINC_FASTEST;
      break;

    case high:
      converter_type = SRC_SINC_MEDIUM_QUALITY;
      break;

    case best:
      converter_type = SRC_SINC_BEST_QUALITY;
      break;

    default:
      cerr << "error, unknown resampling quality." << endl; 
  }
  // converter_type must be one of these:
  // SRC_SINC_BEST_QUALITY       = 0
  // SRC_SINC_MEDIUM_QUALITY     = 1
  // SRC_SINC_FASTEST            = 2
  // SRC_ZERO_ORDER_HOLD         = 3
  // SRC_LINEAR                  = 4
  
  m_src_state = src_new (converter_type, num_channels, &error) ;
  m_src_data = new SRC_DATA;
  
  m_in_buffer_size = 2 * 1024;
  m_out_buffer_size = 3 * 1024;
  
  m_in_buffer = new float[m_in_buffer_size];
  m_out_buffer = new float[m_out_buffer_size];
  
  m_src_data->data_in  = m_in_buffer;
  m_src_data->data_out = m_out_buffer;
  
  m_src_data->input_frames = 0;
  m_src_data->output_frames = 0;
 
  m_in_offset = 0;
  m_out_offset = 0;

  /*m_debug_fd1 = fopen("resampler_output.raw", "w");
  m_data_ptr = new char[4096];*/

  // m_resampled_frame = new CAudioFrame(PAYLOAD_PCM, 3 * 1024);

}


CFloatResampler::~CFloatResampler()
{
  // delete m_resampled_frame;

  /*fclose(m_debug_fd1);
  delete [] m_data_ptr;*/

  delete[] m_in_buffer;

  delete m_src_data;
  src_delete(m_src_state);

  delete[] m_out_buffer;

}




/*!
    \fn CFloatResampler::resampleFrame(CAudioFrame* in_frame)
 */
int CFloatResampler::resampleFrame(CAudioFrame* in_frame, double factor)
{
  int res;
  // src_set_ratio (m_src_state, factor);


  appendFrameToImputBuffer(in_frame);

  m_src_data->output_frames = m_out_buffer_size / m_num_channels;
  m_src_data->src_ratio = factor;
  m_src_data->end_of_input = 0;

 
  res = src_process(m_src_state, m_src_data);
  
  if(res != 0) {
    cerr << "CFloatResampler::resampleFrame: " << src_strerror (res) << endl;
  }

  int num_frames = copyResampledFramesToRingbuffer();

  return num_frames;;
}


int CFloatResampler::reset() {
  int retval = src_reset (m_src_state) ;
  
  m_src_data->data_in  = m_in_buffer;
  m_src_data->data_out = m_out_buffer;
  
  m_src_data->input_frames = 0;
  // m_src_data->output_frames = 0;
  
  m_in_offset = 0;
  m_out_offset = 0;
 
  return retval;
}


void CFloatResampler::appendFrameToImputBuffer(CAudioFrame* in_frame) {

  float* begin_of_free_inbuffer_space = m_src_data->data_in + m_src_data->input_frames;
  int num_of_singlechannel_samples = in_frame->dataSize() / m_size_of_input_singlechannel_sample;

//  src_short_to_float_array((const short*)(in_frame->dataPtr()), begin_of_free_inbuffer_space, num_of_singlechannel_samples);
  src_short_to_float_array((const short*)(in_frame->dataPtr()), m_src_data->data_in, num_of_singlechannel_samples);


  //m_src_data->input_frames += num_of_singlechannel_samples / m_num_channels;
  m_src_data->input_frames = num_of_singlechannel_samples / m_num_channels;
}




int CFloatResampler::copyResampledFramesToRingbuffer() 
{

  if(m_src_data->input_frames_used < m_src_data->input_frames) {
    cerr << "CFloatResampler::resampleFrame: not all input frames were used!" << endl;
    
    // copy remaining input (already tranformed float values) to the beginning of the input buffer and set m_in_offset.   
    int frames_to_copy = m_src_data->input_frames - m_src_data->input_frames_used;
    void* start_addr = m_src_data->data_in + m_src_data->input_frames_used * m_num_channels;
    
    // assert that mem areas do not overlap!
    assert(frames_to_copy < m_src_data->input_frames_used);
    memcpy(m_src_data->data_in, start_addr, frames_to_copy * m_size_of_float_multichannel_sample); 

    m_src_data->input_frames = frames_to_copy;
    
  }
  else {
    m_src_data->input_frames = 0;
  }

  int num_single_channel_samples = m_src_data->output_frames_gen * m_num_channels;
  m_ringbuffer->write(m_src_data->data_out, num_single_channel_samples);

  /*char *start_ptr = m_resampled_frame->dataEndPtr(); */
  

  /*src_float_to_short_array (m_src_data->data_out, 
                            (short*)m_data_ptr, 
                            num_single_channel_samples);

  fwrite(m_data_ptr, 2, num_single_channel_samples, m_debug_fd1); */
  /* m_resampled_frame->dataSizeAdded(num_single_channel_samples * sizeof(short));*/
  
  m_src_data->input_frames_used = 0;
  m_src_data->output_frames_gen = 0;

  return num_single_channel_samples;
}


int CFloatResampler::sizeInMultiChannelSamples() {
  return m_src_data->input_frames;
}

