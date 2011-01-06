/*
 * CMsgProgress.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: martin
 */

#include "CMsgProgress.h"

CMsgProgress::CMsgProgress(uint32_t jobID, uint32_t progress) : m_jobID(jobID), m_progress(progress) {
	m_msgType = E_MSG_PROGRESS;
	m_payloadSize = 4;
	m_msgID = ++CMsgBase::m_last_id;

}

CMsgProgress::CMsgProgress(char* buffer, int size) {
	m_msgType = E_MSG_PROGRESS;
	m_msgID = checkHeader(buffer, size, E_MSG_PROGRESS);

	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(buffer + getHeaderSize());

	m_jobID    = u32PayloadPtr[0];
	m_progress = u32PayloadPtr[1];
}

CMsgProgress::~CMsgProgress() {

}

char* CMsgProgress::serialize(int& size) {
	int payloadSize = sizeof( m_progress );

	size = reallocSerialisationBuffer(payloadSize);

	serializeHeader();
	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(getPayloadBufferPtr());

	u32PayloadPtr[0] = m_jobID;
	u32PayloadPtr[1] = m_progress;

	return getSerialisationBufferPtr();
}

bool CMsgProgress::operator==(const CMsgProgress& other) {
	return ( m_jobID == other.m_jobID && m_progress == other.m_progress && equalTo(other));
}

