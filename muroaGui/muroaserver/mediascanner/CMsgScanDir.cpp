/*
 * CMsgScanDir.cpp
 *
 *  Created on: 1 Nov 2010
 *      Author: martin
 */

#include "CMsgScanDir.h"

CMsgScanDir::CMsgScanDir(std::string path) {
	m_msgType = E_MSG_SCAN_DIR;
	m_payloadSize = path.size();
	m_msgID = ++CMsgBase::m_last_id;

	m_path = path;
}

CMsgScanDir::CMsgScanDir(char* buffer, int size) {
	m_msgType = E_MSG_SCAN_DIR;
	m_msgID = checkHeader(buffer, size, E_MSG_SCAN_DIR);
}

CMsgScanDir::~CMsgScanDir() {
}

bool CMsgScanDir::operator==(const CMsgScanDir &other) {
	if( m_path.compare(other.m_path) == 0 && equalTo(other) ) {
		return true;
	}
	else {
		return false;
	}
}


char* CMsgScanDir::serialize(int& size ) {
	size = reallocSerialisationBuffer(0);
	serializeHeader();

	return getSerialisationBufferPtr();
}
