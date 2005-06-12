/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
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
#ifndef CRINGBUFFER_H
#define CRINGBUFFER_H

/**
Ringbuffer implementation for audio data.

@author Martin Runge
*/
class CRingBuffer{
public:
    CRingBuffer(int size_in_frames, int num_channels = 2);
    ~CRingBuffer();

    char* getWriteBuffer();
    char* read(int bytes);

    int size();

    inline int getNumChannels() { return m_num_channels; };
    inline int getNumFrames() { return m_num_frames; };

    void write(float* src, int num_samples);


private:
    char* m_buffer;
    int m_buffer_size_in_bytes;

    char* m_read_ptr;
    char* m_write_ptr;

    int m_num_channels;
    int m_num_frames;

    
};

#endif
