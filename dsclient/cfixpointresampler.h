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

#include <stdint.h>
#include "cresampler.h"
// #include "CFixPointConverter.h"
#include "cfilterdef.h"

#ifndef MAX
#define MAX(x,y) ((x)>(y) ?(x):(y))
#endif
#ifndef MIN
#define MIN(x,y) ((x)<(y) ?(x):(y))
#endif


class CAudioFrame;
class CRingBuffer;


class CFixPointResampler : public CResampler {
public:
    CFixPointResampler(CRingBuffer *ringbuffer, enum converter_quality quality, int num_channels);
    ~CFixPointResampler();

    void init(int num_channels = 2, double volume = 1.0);

    int resampleFrame(CAudioFrame* in_frame, double factor = 1.0);

    int reset();

    int sizeInMultiChannelSamples();
    

    
private:
  void deleteBuffers(void);

  void addSamplesInX(CAudioFrame* in_frame);
  void resample(float factor);
  int copyResampledFramesToRingbuffer();

  long resampleChannelLinear(float factor, int ch, long time);
  long resampleChannelWithFilter(float factor, int ch, long time);

  int FilterDown( int16_t *Xp, int16_t Ph, int Inc, uint16_t dhb);
  int FilterUp( int16_t *Xp, int16_t Ph, int Inc);
  
  int16_t int32toint16(int32_t int32var, int n_fix_point_bits);

  int m_frames_resampled;

  // offsets are given in number of multichannel samples
  // e.g. one sample of each channel (right and left for 2 ch stereo) would result in an ofset of 1.

  float m_max_factor;
  float m_min_factor;

  bool    m_interp_filter;

  int16_t  ** m_X;  /* per channel input buffers */
  int16_t  ** m_Y;  /* per channel output buffers */


/* Layout of per channel input buffer. All indices and size are in samples.
 * 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | reuse samples | data to resample / unused input samples | free space  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |               |                                         |             |
 * m_X[ch]         m_X_read                                  m_X_write     m_X_size 
 *
 *
 * At the beginning of each buffer (m_X[ch]), between index 0 and m_X_read,
 * there are either silence samples
 * or old samples, that have already been resampled. They are needed for the 
 * for windowfilter when resampling the following samples.
 *
 * The buffer is filled up to index m_X_write - 1 (last sample in buffer), 
 * but not all samples can be used for resampling immediately, as the 
 * windowfilter looks also into the future. So new samples have to be witten
 * into the buffer as soon as we have used index 
 * m_X_write - 1 - windowfiltersize / 2
 *
 */

  int m_X_size;   /* size of per channel input buffer in samples  */
  int* m_X_read;   /* per channel read position / index of sample to read next */
  int* m_X_write;  /* per channel write position / index for next sample to write into buffer */

  int* m_Y_read;   /* per channel output buffer read position / index of first sample to read out */
  int* m_Y_write;  /* per channel output buffer write position / write next calculated sample here */ 
  int m_Y_size;   /* size of the output buffer */

  int m_max_windowfilter_range;  /* how many input samples before 
                                    and after the actual sample are 
                                    needed for the windowfilter */

  uint32_t  m_time;

  bool      m_initial;

  double    m_factor;
  double    m_volume;

  int m_num_channels;

  const CFilterDef m_large_filter, m_small_filter;

  const CFilterDef * m_filter;
  

  int m_size_of_float_multichannel_sample;     // called frame in libsamplerate
  int m_size_of_input_singlechannel_sample;
  int m_size_of_input_multichannel_sample;

  // CAudioFrame* m_resampled_frame;
  CRingBuffer* m_ringbuffer;

  enum converter_quality  m_quality;

  // for debugging 
  FILE *m_debug_fd1;
  FILE *m_debug_fd2;
  FILE *m_debug_fd3;
};

#endif
