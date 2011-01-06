/*
 * CMsgFinished.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: martin
 */

#include "CMsgFinished.h"

CMsgFinished::CMsgFinished(uint32_t jobID) : m_jobID(jobID) {
	m_msgType = E_MSG_FINISHED;
	m_payloadSize = sizeof(m_jobID);
	m_msgID = ++CMsgBase::m_last_id;
}

CMsgFinished::CMsgFinished(char* buffer, int size) {
	m_msgType = E_MSG_FINISHED;
	m_msgID = checkHeader(buffer, size, E_MSG_FINISHED);

	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(buffer + getHeaderSize());

	m_jobID    = u32PayloadPtr[0];
}

CMsgFinished::~CMsgFinished() {

}

char* CMsgFinished::serialize(int& size ) {
	size = reallocSerialisationBuffer(0);
	serializeHeader();

	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(getPayloadBufferPtr());
	u32PayloadPtr[0] = m_jobID;

	return getSerialisationBufferPtr();
}

bool CMsgFinished::operator==(const CMsgFinished& other) {
	return ( m_jobID == other.m_jobID && equalTo(other));
}

