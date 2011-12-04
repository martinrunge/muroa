/*
 * CMsgQuit.cpp
 *
 *  Created on: 31 Oct 2010
 *      Author: martin
 */

#include "CMsgQuit.h"

#include <sstream>

CMsgQuit::CMsgQuit() {
	m_msgType = E_MSG_QUIT;
	m_payloadSize = 0;
	m_msgID = ++CMsgBase::m_last_id;
}

CMsgQuit::CMsgQuit(char* buffer, int size) {
	m_msgType = E_MSG_QUIT;
	m_msgID = checkHeader(buffer, size, E_MSG_QUIT);
}


CMsgQuit::~CMsgQuit() {
}

bool CMsgQuit::operator==(const CMsgQuit& other) {
	return (equalTo(other));
}

char* CMsgQuit::serialize(int& size ) {

	size = reallocSerialisationBuffer(0);
	serializeHeader();

	return getSerialisationBufferPtr();
}
