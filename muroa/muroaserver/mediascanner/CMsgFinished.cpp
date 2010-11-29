/*
 * CMsgFinished.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: martin
 */

#include "CMsgFinished.h"

CMsgFinished::CMsgFinished() {
	m_msgType = E_MSG_FINISHED;
	m_payloadSize = 0;
	m_msgID = ++CMsgBase::m_last_id;
}

CMsgFinished::CMsgFinished(char* buffer, int size) {
	m_msgType = E_MSG_FINISHED;
	m_msgID = checkHeader(buffer, size, E_MSG_FINISHED);
}

CMsgFinished::~CMsgFinished() {

}

char* CMsgFinished::serialize(int& size ) {
	size = reallocSerialisationBuffer(0);
	serializeHeader();

	return getSerialisationBufferPtr();
}

bool CMsgFinished::operator==(const CMsgFinished& other) {
	return (equalTo(other));
}

