/*
 * CMsgCollectionChanged.cpp
 *
 *  Created on: 6 Jan 2011
 *      Author: martin
 */

#include "CMsgCollectionChanged.h"

CMsgCollectionChanged::CMsgCollectionChanged(uint32_t newRev, uint32_t minRev, uint32_t maxRev):
                                                                                  m_newRev(newRev),
                                                                                  m_minRev(minRev),
                                                                                  m_maxRev(maxRev) {
	m_msgType = E_MSG_COLLECTION_CHANGED;
	m_payloadSize = 3 * sizeof(uint32_t);
	m_msgID = ++CMsgBase::m_last_id;
}

CMsgCollectionChanged::CMsgCollectionChanged(char* buffer, int size) {
	m_msgType = E_MSG_COLLECTION_CHANGED;
	m_msgID = checkHeader(buffer, size, E_MSG_COLLECTION_CHANGED);

	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(buffer + getHeaderSize());

	m_newRev = u32PayloadPtr[0];
	m_minRev = u32PayloadPtr[1];
	m_maxRev = u32PayloadPtr[2];
}

CMsgCollectionChanged::~CMsgCollectionChanged() {

}



char* CMsgCollectionChanged::serialize(int& size) {
	int payloadSize = sizeof( m_newRev ) + sizeof( m_minRev ) + sizeof( m_maxRev );

	size = reallocSerialisationBuffer(payloadSize);

	serializeHeader();
	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(getPayloadBufferPtr());

	u32PayloadPtr[0] = m_newRev;
	u32PayloadPtr[1] = m_minRev;
	u32PayloadPtr[2] = m_maxRev;

	return getSerialisationBufferPtr();
}

bool CMsgCollectionChanged::operator==(const CMsgCollectionChanged& other) {
	return ( m_newRev == other.m_newRev &&
			 m_minRev == other.m_minRev &&
			 m_maxRev == other.m_maxRev &&
			 equalTo(other));
}

