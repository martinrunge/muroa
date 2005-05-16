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
#ifndef CRTPPACKET_H
#define CRTPPACKET_H

/**
@author Martin Runge
*/


#include <sys/types.h> 


typedef struct rtp_header_bits {
    int version:2;
    int padding:1;
    int extension:1;
    int CSRC_count:4;
    int marker:1;
    int payload_type:7;
    unsigned short sequence_number;
    unsigned long timestamp;
    unsigned long SSRC; 
  } rtp_header_bits_t;


union rtp_header_t {
  long three32bitwords[3];
  rtp_header_bits_t rtp_header_bits;
};


enum payload_type_t {
  PAYLOAD_UNKNOWN,
  PAYLOAD_SYNC_OBJ,
  PAYLOAD_PCM,
  PAYLOAD_VORBIS,
  PAYLOAD_MP3,
  PAYLOAD_FLAC
 
};


class CRTPPacket{
public:
    CRTPPacket(char *buffer, int buffer_size, bool delete_buffer_in_dtor = true);

    CRTPPacket(int payload_size = 1024, bool delete_buffer_in_dtor = true);

    ~CRTPPacket();

    /** Pointer to the data buffer that holds the whole packet. That is RTP header and payload. 
        can be used for sending and receiving rtp packets over the network */
    inline char* bufferPtr(void) { return m_buffer; };

    /** The size of the whole memory buffer that can be used to receive a RTP packet via network */
    inline int bufferSize(void) { return m_buffer_size; };

    /** Size of the buffer taht is used by data. It is the size of the RTP header plus the size of the payload actually loaded */
    inline int usedBufferSize(void) { return sizeof(rtp_header_t) + m_used_payload_size; };

    /** Set size of the buffer that is used by data. It is the size of the RTP header plus the size of the payload actually loaded */
    void usedBufferSize(int size);

    inline char* payloadBufferPtr(void) { return m_payload_ptr; };
    inline int payloadBufferSize(void) { return m_payload_size; };

    inline int usedPayloadBufferSize(void) { return m_used_payload_size; };
    void usedPayloadBufferSize(int used);    

    inline enum payload_type_t payloadType() { return (enum payload_type_t) m_rtp_header->rtp_header_bits.payload_type; };
    inline void payloadType(enum payload_type_t pl) { m_rtp_header->rtp_header_bits.payload_type = (int)pl; };

    inline unsigned short seqNum() { return m_rtp_header->rtp_header_bits.sequence_number; };
    inline void seqNum(unsigned short sn) { m_rtp_header->rtp_header_bits.sequence_number = sn; };

    inline unsigned long timestamp() { return m_rtp_header->rtp_header_bits.timestamp; };
    inline void timestamp(unsigned long ts) { m_rtp_header->rtp_header_bits.timestamp = ts; };

    int copyInPayload(const char* from, const int size);

private:

    void init(void);
    CRTPPacket operator=(CRTPPacket packet);

    rtp_header_t  *m_rtp_header; 

    char *m_buffer;
    int m_buffer_size;

    char *m_payload_ptr;
    int m_payload_size;       
    int m_used_payload_size;

    bool m_delete_buffer_in_dtor;
  
};

#endif
