/*
 * CMsgResponse.cpp
 *
 *  Created on: 31 Oct 2010
 *      Author: martin
 */

#include "CMsgResponse.h"

CMsgResponse::CMsgResponse(int requestID, int retCode) {
	m_msgType = E_MSG_RESP;
	m_payloadSize = 0;
	m_msgID = ++CMsgBase::m_last_id;

	m_requestID = requestID;
	m_retCode = retCode;
}

CMsgResponse::CMsgResponse(char *buffer, int size) {
	m_msgType = E_MSG_RESP;
	m_msgID = checkHeader(buffer, size, E_MSG_RESP);

	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(buffer + getHeaderSize());

	m_requestID = u32PayloadPtr[0];
	m_retCode = u32PayloadPtr[1];
}


CMsgResponse::~CMsgResponse() {
}

bool CMsgResponse::operator==(const CMsgResponse &other) {
	return ( equalTo(other) && m_requestID == other.m_requestID && m_retCode == other.m_retCode );
}


char* CMsgResponse::serialize(int& size ) {

	int payloadSize = sizeof( m_requestID ) + sizeof( m_retCode );

	size = reallocSerialisationBuffer(payloadSize);

	serializeHeader();
	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(getPayloadBufferPtr());

	u32PayloadPtr[0] = m_requestID;
	u32PayloadPtr[1] = m_retCode;

	return getSerialisationBufferPtr();
}
