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
#ifndef CFIXPOINTRESAMPLER_H
#define CFIXPOINTRESAMPLER_H

/**
audio resampling class

@author Martin Runge
*/


#include "cresampler.h"
#include "CFixPointConverter.h"



class CAudioFrame;
class CRingBuffer;


class CFixPointResampler : public CResampler {
public:
    CFixPointResampler(CRingBuffer *ringbuffer, enum converter_quality quality, int num_channels);
    ~CFixPointResampler();
    
    int resampleFrame(CAudioFrame* in_frame, double factor = 1.0);
    int reset();

    int sizeInMultiChannelSamples();

    
private:
  
  CFixPointConverter* m_converter;

  short *m_in_buffer, *m_out_buffer;
  int    m_in_buffer_size, m_out_buffer_size;
  

  // offsets are given in number of multichannel samples
  // e.g. one sample of each channel (right and left for 2 ch stereo) would result in an ofset of 1.
  int m_in_offset;
  int m_out_offset;
  
  int m_num_channels;
  

  int m_size_of_float_multichannel_sample;     // called frame in libsamplerate
  int m_size_of_input_singlechannel_sample;
  int m_size_of_input_multichannel_sample;

  // CAudioFrame* m_resampled_frame;
  CRingBuffer* m_ringbuffer;

  void appendFrameToImputBuffer(CAudioFrame* in_frame);
  int copyResampledFramesToRingbuffer();

  // for debugging 
  char* m_data_ptr;
  FILE *m_debug_fd1;
  FILE *m_debug_fd2;
  FILE *m_debug_fd3;
};

#endif
