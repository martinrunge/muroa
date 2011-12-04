/*
 * CMsgResponse.cpp
 *
 *  Created on: 31 Oct 2010
 *      Author: martin
 */

#include "CMsgResponse.h"
#include <string.h>   // memcpy declared here

using namespace std;

CMsgResponse::CMsgResponse(int requestID, int retCode, string message) {
	m_msgType = E_MSG_RESP;
	m_msgID = ++CMsgBase::m_last_id;

	m_requestID = requestID;
	m_retCode = retCode;
	m_message = message;

	m_payloadSize = sizeof(m_requestID) + sizeof(m_retCode) + m_message.size();
}

CMsgResponse::CMsgResponse(char *buffer, int size) {
	m_msgType = E_MSG_RESP;
	m_msgID = checkHeader(buffer, size, E_MSG_RESP);

	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(buffer + getHeaderSize());

	m_requestID = u32PayloadPtr[0];
	m_retCode = u32PayloadPtr[1];

	int messageoffset = sizeof(m_requestID) + sizeof(m_retCode);

	m_message = string(buffer + getHeaderSize() + messageoffset, m_payloadSize - messageoffset);
}


CMsgResponse::~CMsgResponse() {
}

bool CMsgResponse::operator==(const CMsgResponse &other) {
	return ( equalTo(other) && m_requestID == other.m_requestID && m_retCode == other.m_retCode && m_message.compare(other.m_message) == 0);
}


char* CMsgResponse::serialize(int& size ) {

	int payloadSize = sizeof( m_requestID ) + sizeof( m_retCode ) + m_message.size();

	size = reallocSerialisationBuffer(payloadSize);

	serializeHeader();
	uint32_t* u32PayloadPtr = reinterpret_cast<uint32_t*>(getPayloadBufferPtr());

	u32PayloadPtr[0] = m_requestID;
	u32PayloadPtr[1] = m_retCode;

	memcpy( getPayloadBufferPtr() + sizeof( m_requestID ) + sizeof( m_retCode ), m_message.data(), m_message.size() );

	return getSerialisationBufferPtr();
}
