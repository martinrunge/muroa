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
#include "cringbuffer.h"
#include "assert.h"

#include <string.h>

CRingBuffer::CRingBuffer(int size_in_frames, int num_channels)
{
  m_num_frames = size_in_frames;
  m_num_channels = num_channels;
  
  m_buffer_size_in_bytes = m_num_frames * m_num_channels * 2;

  m_buffer = new char[m_buffer_size_in_bytes];

  m_write_ptr = m_buffer;
  m_read_ptr = m_buffer;

}


CRingBuffer::~CRingBuffer()
{
  delete [] m_buffer;
}


/*!
    \fn CRingBuffer::getWriteBuffer()
 */
char* CRingBuffer::getWriteBuffer()
{
    return m_write_ptr;
}


/*!
    \fn CRingBuffer::read(int bytes)
 */
char* CRingBuffer::read(int bytes)
{
    char* buffer = new char [bytes];
    
    assert(bytes < size());
    
    if(m_write_ptr >= m_read_ptr) { // normal case
      memcpy(buffer, m_read_ptr, bytes);
      m_read_ptr += bytes;
    }  
    else {  // write_pre had overflow at buffer end, read pointer not yet.
      int bytes_to_overflow = (m_buffer + m_buffer_size_in_bytes) - m_read_ptr;
      if(bytes_to_overflow > bytes) {  // we can do it in one piece
        memcpy(buffer, m_read_ptr, bytes);
        m_read_ptr += bytes;
      }
      else { // ned to do it in two pieces
        memcpy(buffer, m_read_ptr, bytes_to_overflow);
        int rest = bytes - bytes_to_overflow;
        memcpy(buffer + bytes_to_overflow, m_buffer, rest);
        m_read_ptr = m_buffer + rest;
      }
    }
    return buffer;
}


/*!
    \fn CRingBuffer::size()
 */
int CRingBuffer::size()
{
    int size;
    if(m_write_ptr >= m_read_ptr)
      size = m_write_ptr - m_read_ptr;
    else
      size = m_buffer_size_in_bytes - (m_read_ptr - m_write_ptr); 

    return size;
}


/*!
    \fn CRingBuffer::write(float* src, int num_samples)
 */
void CRingBuffer::write(float* src, int num_samples)
{
  float scaled_value_float;
  short scaled_value_int;

  assert(num_samples * 2 < m_buffer_size_in_bytes - size());

  // range in 16 bit LE singned is -32768 ... 32767

  while (num_samples) { 
    num_samples--;
    assert(src[num_samples] <= 1.0 && src[num_samples] >= -1.0);

    scaled_value_float = src[num_samples] * 32767;
    scaled_value_int = (short)scaled_value_float;

        
    *m_write_ptr++ = scaled_value_int;
    assert(m_write_ptr != m_read_ptr);

    if(m_write_ptr - m_buffer >= m_buffer_size_in_bytes) {
      m_write_ptr = m_buffer;    
      assert(m_write_ptr != m_read_ptr);
    }
  }
}
