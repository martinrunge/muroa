/*
 * CPacketRingBuffer.cpp
 *
 *  Created on: 02.01.2016
 *      Author: martin
 */

#include "CPacketRingBuffer.h"

#include "CPerSSRCRingBuffer.h"
#include "crtppacket.h"

#include <CApp.h>

#include "assert.h"

using namespace std;
using namespace muroa;

CPacketRingBuffer::CPacketRingBuffer(int num_ssrc) : m_num_ssrc(num_ssrc) {

}

CPacketRingBuffer::~CPacketRingBuffer() {
	m_buffer_map.clear();
}


void CPacketRingBuffer::appendRTPPacket(CRTPPacket* packet) {
	uint32_t ssrc = packet->ssrc();
	map<uint32_t, CPerSSRCRingBuffer*>::iterator it;
	CPerSSRCRingBuffer* rb;

	it = m_buffer_map.find(ssrc);
	if( it == m_buffer_map.end() ) {
		// new ssrc appeared. Create new perSSRC ringbuffer for it and remove the oldest unused one if there would be more than m_num_ssrc otherwise.
		LOG4CPLUS_DEBUG( CApp::logger(), "CPacketRingBuffer::appendRTPPacket: Adding new ringbuffer for ssrc=" << ssrc);
		if(m_buffer_map.size() >= m_num_ssrc ) {
			eraseOldestInactiveSSRC();
		}
		rb = new CPerSSRCRingBuffer(ssrc);
		m_buffer_map.insert( pair<uint32_t, CPerSSRCRingBuffer*>(ssrc, rb) );
	}
	else {
		rb = it->second;
	}
	rb->appendRTPPacket(packet);
}

CRTPPacket* CPacketRingBuffer::readPacket(uint32_t ssrc) {
	map<uint32_t, CPerSSRCRingBuffer*>::iterator it;
	CPerSSRCRingBuffer* rb;

	it = m_buffer_map.find(ssrc);
	if( it == m_buffer_map.end() ) {
		return nullptr;
	}
	else {
		rb = it->second;
		return rb->readPacket();
	}
}

int CPacketRingBuffer::getRingbufferSize(uint32_t ssrc) {
	map<uint32_t, CPerSSRCRingBuffer*>::iterator it;
	CPerSSRCRingBuffer* rb;

	it = m_buffer_map.find(ssrc);
	if( it == m_buffer_map.end() ) {
		return 0;
	}
	else {
		rb = it->second;
		return rb->getRingbufferSize();
	}
}

void CPacketRingBuffer::clear(uint32_t ssrc) {
	map<uint32_t, CPerSSRCRingBuffer*>::iterator it;
	CPerSSRCRingBuffer* rb;

	it = m_buffer_map.find(ssrc);
	if( it == m_buffer_map.end() ) {
		return;
	}
	else {
		rb = it->second;
		delete rb;
		m_buffer_map.erase(it);
	}
}



uint32_t CPacketRingBuffer::eraseOldestInactiveSSRC() {

	assert( m_buffer_map.size() >= 2 );

	map<uint32_t, CPerSSRCRingBuffer*>::iterator it = m_buffer_map.begin();

	uint32_t oldest_inactive_ssrc = it->second->ssrc();
	if(oldest_inactive_ssrc == m_active_ssrc ) {
		it++;
		uint32_t oldest_inactive_ssrc = it->second->ssrc();
	}
	struct timespec oldest_inactive_ts = it->second->age();

	for(it = m_buffer_map.begin(); it != m_buffer_map.end(); it++) {
		if( it->second->ssrc() != m_active_ssrc && it->second->age() < oldest_inactive_ts ) {
			oldest_inactive_ts = it->second->age();
			oldest_inactive_ssrc = it->second->ssrc();
		}
	}

	m_buffer_map.erase(oldest_inactive_ssrc);

}
