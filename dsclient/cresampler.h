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



class CAudioFrame;
class CRingBuffer;

enum converter_quality {
  poor,
  medium,
  high,
  best
};

class CResampler{
public:
    CResampler(CRingBuffer *ringbuffer, enum converter_quality quality, int num_channels);
    virtual ~CResampler() = 0;
    
    virtual int resampleFrame(CAudioFrame* in_frame, double factor = 1.0) = 0;
    virtual int reset() = 0;

    virtual int sizeInMultiChannelSamples() = 0;
    
};

#endif
