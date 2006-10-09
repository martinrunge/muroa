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
#include <iostream>

#include <limits.h>
#include <math.h>

using namespace std;

CRingBuffer::CRingBuffer(int size_in_frames, int num_channels)
{
  m_num_frames = size_in_frames;
  m_num_channels = num_channels;
  
  m_buffer_size_in_bytes = m_num_frames * m_num_channels * sizeof(short);

  m_buffer = new char[m_buffer_size_in_bytes];

  m_write_ptr = m_buffer;
  m_read_ptr = m_buffer;

  m_debug_fd0 = fopen("ringbuffer_data.raw", "w");
  m_debug_fd1 = fopen("ringbuffer_input.raw", "w");
  m_debug_fd2 = fopen("ringbuffer_output.raw", "w");
  //m_data_ptr = new char[4096];


}


CRingBuffer::~CRingBuffer()
{
  fclose(m_debug_fd0);
  fclose(m_debug_fd1);
  fclose(m_debug_fd2);
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
    
    if(!(bytes <= size())) {
      cerr << "CRingBuffer::read: bytes = " << bytes << " size = " << size() << endl;
    }
    assert(bytes <= size());
    
    if(m_write_ptr >= m_read_ptr) { // normal case
      memcpy(buffer, m_read_ptr, bytes);
      m_read_ptr += bytes;
    }  
    else {  // write_ptr had overflow at buffer end, read pointer not yet.
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
    fwrite(buffer, 1, bytes, m_debug_fd2);
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

int CRingBuffer::sizeInMultiChannelSamples() {
  return size() / (m_num_channels * sizeof(short));
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
  // scale float [-1.0 .. 1.0] to what a long int can carry and convert it to long int with lrintf
  // at the end, shift the long int down to a short int.

  for(int i=0; i < num_samples; i++) {
    if(src[i] <= 1.1 && src[i] >= -1.1) { // normal case, do the calculation
        scaled_value_float = src[i] * (0.8 * (1<<31) );
        scaled_value_int = (lrintf(scaled_value_float) >> 16);
    }
    else {  // if the float value excedded the range [-1 ... 1], clip it
      if(src[i] > 1.1) {
        cerr << "CRingBuffer::write: sample cut off (+) : " << src[i] << endl;
        scaled_value_int = SHRT_MAX;
      } 
      else { // (src[i] < -1.0)
        cerr << "CRingBuffer::write: sample cut off (-) : " << src[i] << endl;
        scaled_value_int = SHRT_MIN;
      }
    }


    // fwrite(&scaled_value_int, 2, 1, m_debug_fd1);
        
    *(short*)m_write_ptr = scaled_value_int;
    m_write_ptr += sizeof(short);
    assert(m_write_ptr != m_read_ptr);

    if(m_write_ptr - m_buffer >= m_buffer_size_in_bytes) {
      m_write_ptr = m_buffer;    
      assert(m_write_ptr != m_read_ptr);
    }
  }
}


/*!
    \fn CRingBuffer::write(unsigned short* buffer, int num_samples)
 */
void CRingBuffer::write(short* buffer, int num_samples)
{
  short scaled_value_int;

  assert(num_samples * 2 < m_buffer_size_in_bytes - size());

  for(int i=0; i < num_samples; i++) {

    // fwrite(&scaled_value_int, 2, 1, m_debug_fd1);
        
    *(short*)m_write_ptr = buffer[i];
    m_write_ptr += sizeof(short);
    assert(m_write_ptr != m_read_ptr);

    if(m_write_ptr - m_buffer >= m_buffer_size_in_bytes) {
      m_write_ptr = m_buffer;    
      assert(m_write_ptr != m_read_ptr);
    }
  }
}


/*!
    \fn CRingBuffer::write(int16** per_channel_buffers, int num_samples_per_channel )
 */
int CRingBuffer::write(int16_t** per_channel_buffers, int start_offset, int end_offset )
{
  int16_t* writeptr = reinterpret_cast<int16_t*>(m_write_ptr);
  int16_t* bufferptr = reinterpret_cast<int16_t*>(m_buffer);
  int16_t* endptr = bufferptr + m_buffer_size_in_bytes / sizeof(int16_t) - 1;

  assert(available() >= (end_offset - start_offset) * m_num_channels * sizeof(int16_t));

  for(int i = start_offset; i < end_offset; i++) {
    for(int ch = 0; ch < m_num_channels; ch++) {
      *writeptr = per_channel_buffers[ch][i];
      fwrite(writeptr, 1, sizeof(int16_t), m_debug_fd0);
      if(writeptr < endptr)
        writeptr++;
      else
        writeptr = bufferptr;
    }
  } 
   
  int size_in_bytes = (end_offset - start_offset) * m_num_channels * sizeof(int16_t);
  
  char *tmp_ptr = m_write_ptr;
  int bytes_to_end, rest;
  
  bytes_to_end = (m_buffer + m_buffer_size_in_bytes) - tmp_ptr;
  
  if(bytes_to_end > size_in_bytes) {
    for(int i = 0; i < size_in_bytes; i++) {
      fwrite(tmp_ptr + i, 1, 1, m_debug_fd1); 
    }
  }
  else {
    for(int i = 0; i < bytes_to_end; i++) {
      fwrite(tmp_ptr + i, 1, 1, m_debug_fd1); 
    }
    
    rest = size_in_bytes - bytes_to_end;
    for(int j = 0; j < rest; j++) {
      fwrite(m_buffer + j, 1, 1, m_debug_fd1); 
    }
  }
  
  
  m_write_ptr = reinterpret_cast<char*>(writeptr);    

  return (end_offset - start_offset);
}


/*!
    \fn CRingBuffer::capacity(void)
 */
int CRingBuffer::capacity(void)
{
  return m_buffer_size_in_bytes;
}


/*!
    \fn CRingBuffer::available(void)
 */
int CRingBuffer::available(void)
{
	return m_buffer_size_in_bytes - size();
}
