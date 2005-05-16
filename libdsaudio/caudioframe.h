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
#ifndef CAUDIOFRAME_H
#define CAUDIOFRAME_H

/**
Class  torepresent a frame of audio data. This can be a vorbis frame, mp3 frame, flac frame or just an amount of PCM data. To minimize use of new and delete, the frame class only uses a pointer to th beginning of the frame in the ringbuffer, the size of the frame and its payload type so it can be directed to the right decoder.

@author Martin Runge
*/

#include "crtppacket.h"

class CRTPPacket;

class CAudioFrame{
public:
    CAudioFrame(enum payload_type_t payload_type, int max_frame_size = 4096);

    CAudioFrame(CRTPPacket* rtp_packet, int max_frame_size = 4096);

    ~CAudioFrame();

    void copyData(const char* data, const int data_len);
    void moveDataToBegin(const int from);

    
    // inline void dataPtr(char* ptr) { m_frame_data = ptr; };
    inline char* dataPtr(void) { return m_frame_data; };

    char* dataEndPtr(void);
    void dataSizeAdded(int num_additional_bytes);

    inline void dataSize(int size) { m_frame_data_size = size; };
    inline int dataSize(void) { return m_frame_data_size; };

    inline void frameType(enum payload_type_t type) { m_frame_type = type; };
    inline enum payload_type_t frameType(void) { return m_frame_type; };

    inline void firstSampleNr(long nr) { m_first_sample_nr = nr; };
    inline long firstSampleNr(void) { return m_first_sample_nr; };

    inline int maxFrameSize(void) { return m_max_size; };
        
protected:
    char* m_frame_data;
    int m_frame_data_size;
    enum payload_type_t m_frame_type;
    
    int m_max_size;

private:
    long m_first_sample_nr;    


};

#endif
