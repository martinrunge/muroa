/*
 * CMsgOpenDb.cpp
 *
 *  Created on: 28 Nov 2010
 *      Author: martin
 */

#include "CMsgOpenDb.h"

#include <string.h>

using namespace std;

CMsgOpenDb::CMsgOpenDb(std::string db_path) {
	m_msgType = E_MSG_OPEN_DB;
	m_payloadSize = db_path.size();
	m_msgID = ++CMsgBase::m_last_id;

	m_db_path = db_path;
}

CMsgOpenDb::CMsgOpenDb(char* buffer, int size) {
	m_msgType = E_MSG_OPEN_DB;
	m_msgID = checkHeader(buffer, size, E_MSG_OPEN_DB);

	m_db_path = string(buffer + getHeaderSize(), size - getHeaderSize());
}

CMsgOpenDb::~CMsgOpenDb() {
}


bool CMsgOpenDb::operator==(const CMsgOpenDb &other) {
	if( m_db_path.compare(other.m_db_path) == 0 && equalTo(other) ) {
		return true;
	}
	else {
		return false;
	}
}

char* CMsgOpenDb::serialize(int& size ) {
	size = reallocSerialisationBuffer( m_db_path.size() );
	serializeHeader();

	memcpy( getPayloadBufferPtr(), m_db_path.data(), m_db_path.size() );
	return getSerialisationBufferPtr();
}
