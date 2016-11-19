/*
 * CPacketRingBuffer.h
 *
 *  Created on: 02.01.2016
 *      Author: martin
 */

#ifndef MUROAD_CPACKETRINGBUFFER_H_
#define MUROAD_CPACKETRINGBUFFER_H_

#include <cstdint>
#include <map>

class CRTPPacket;
class CPerSSRCRingBuffer;

class CPacketRingBuffer {
public:
	CPacketRingBuffer(int num_ssrc);
	virtual ~CPacketRingBuffer();

    void appendRTPPacket(CRTPPacket* packet);

    CRTPPacket* readPacket(uint32_t ssrc);
    int getRingbufferSize(uint32_t ssrc);

    uint32_t getActiveSSRC() { return m_active_ssrc; };
    void     setActiveSSRC(uint32_t ssrc) { m_active_ssrc = ssrc; };

    void clear(uint32_t ssrc);

private:
    std::map<uint32_t, CPerSSRCRingBuffer*> m_buffer_map;
    uint32_t m_active_ssrc;
    int m_num_ssrc;

    uint32_t eraseOldestInactiveSSRC();
};

#endif /* MUROAD_CPACKETRINGBUFFER_H_ */
