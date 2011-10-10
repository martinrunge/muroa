/*
 * CMsgError.cpp
 *
 *  Created on: 11 Apr 2011
 *      Author: martin
 */

#include "CMsgError.h"

using namespace std;

#include <cstring>

CMsgError::CMsgError(int32_t errorcode, std::string message) {
	m_msgType = E_MSG_ERR;
	m_msgID = ++CMsgBase::m_last_id;

	m_error_code = errorcode;
	m_message = message;

	m_payloadSize = sizeof(m_error_code) + m_message.size();
}

CMsgError::CMsgError(char* buffer, int size) {
	m_msgType = E_MSG_ERR;
	m_msgID = checkHeader(buffer, size, E_MSG_ERR);

	int32_t* u32PayloadPtr = reinterpret_cast<int32_t*>(buffer + getHeaderSize());

	m_error_code = u32PayloadPtr[0];

	int messageoffset = sizeof(m_error_code);

	m_message = string(buffer + getHeaderSize() + messageoffset, m_payloadSize - messageoffset);

}


CMsgError::~CMsgError() {
}

bool CMsgError::operator==(const CMsgError& other) {
	return ( equalTo(other) && m_error_code == other.m_error_code && m_message.compare(other.m_message) == 0);
}

char* CMsgError::serialize(int& size ) {
	int payloadSize = sizeof( m_error_code ) + m_message.size();

	size = reallocSerialisationBuffer(payloadSize);

	serializeHeader();
	int32_t* u32PayloadPtr = reinterpret_cast<int32_t*>(getPayloadBufferPtr());

	u32PayloadPtr[0] = m_error_code;

	memcpy( getPayloadBufferPtr() + sizeof( m_error_code ), m_message.data(), m_message.size() );

	return getSerialisationBufferPtr();
}
