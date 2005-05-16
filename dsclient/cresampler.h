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
#ifndef CRESAMPLER_H
#define CRESAMPLER_H

/**
audio resampling class

@author Martin Runge
*/


#include <samplerate.h>


class CAudioFrame;


class CResampler{
public:
    CResampler(int converter_type, int num_channels);
    ~CResampler();
    
    CAudioFrame* resampleFrame(CAudioFrame* in_frame, float factor = 1.0);
    int reset();
    
private:
  SRC_STATE* m_src_state;
  SRC_DATA* m_src_data;  
  
  float *m_in_buffer, *m_out_buffer;
  int    m_in_buffer_size, m_out_buffer_size;
  

  // offsets are given in number of multichannel samples
  // e.g. one sample of each channel (right and left for 2 ch stereo) would result in an ofset of 1.
  int m_in_offset;
  int m_out_offset;
  
  int m_num_channels;
  

  int m_size_of_float_multichannel_sample;     // called frame in libsamplerate
  int m_size_of_input_singlechannel_sample;
  int m_size_of_input_multichannel_sample;

  CAudioFrame* m_resampled_frame;

  void appendFrameToImputBuffer(CAudioFrame* in_frame);
  void copyResampledFramesToResampledFrame();

  FILE *m_debug_fd1;
  FILE *m_debug_fd2;
  FILE *m_debug_fd3;
};

#endif
