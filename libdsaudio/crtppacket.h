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
#include <inttypes.h>


typedef struct rtp_header_bits {
    unsigned version:2;
    unsigned padding:1;
    unsigned extension:1;
    unsigned CSRC_count:4;
    unsigned marker:1;
    unsigned payload_type:7;
    uint16_t sequence_number;
    uint32_t timestamp;
    uint32_t SSRC; 
  } rtp_header_bits_t;

typedef struct rtp_header_extension {
    uint16_t defined_by_profile;
    uint16_t num_32bit_words_following;
    uint32_t ds_session_id;
    uint32_t ds_stream_id;
  } rtp_header_extension_t;



union rtp_header_t {
  uint32_t three32bitwords[3];
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

    CRTPPacket(unsigned long session_id = 0, unsigned long stream_id = 0, int payload_size = 1024, bool delete_buffer_in_dtor = true);

    ~CRTPPacket();

    /** Pointer to the data buffer that holds the whole packet. That is RTP header and payload. 
        can be used for sending and receiving rtp packets over the network */
    inline char* bufferPtr(void) { return m_buffer; };

    /** The size of the whole memory buffer that can be used to receive a RTP packet via network */
    inline int bufferSize(void) { return m_buffer_size; };

    /** after writing a rtp packet into the buffer that was retuned by bufferPtr(), call commit to tell how many bytes have actually been written. Then, the RTP packet is parsed. */
    int commit(int num);

    /** Size of the buffer taht is used by data. It is the size of the RTP header plus the size of the payload actually loaded */
    int usedBufferSize(void);

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

    unsigned long sessionID(void);
    void sessionID(unsigned long session_id);
      

    unsigned long streamID(void);
    void streamID(unsigned long stream_id); 
      

    int copyInPayload(const char* from, const int size);

private:

    void init(void);
    CRTPPacket operator=(CRTPPacket packet);

    rtp_header_t  *m_rtp_header;
    rtp_header_extension_t *m_rtp_header_extension; 

    unsigned m_num_csrc;

    char *m_buffer;
    int m_buffer_size;

    char *m_payload_ptr;
    int m_payload_size;       
    int m_used_payload_size;

    bool m_delete_buffer_in_dtor;

    static int m_counter;
  
public:
    static const unsigned short DS_RTP_PROFILE;
};

#endif
