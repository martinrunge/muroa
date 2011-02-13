/*
 * CSubProcessTest.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CSubProcessTest.h"
#include "../CMediaScannerCtrl.h"

#include "../mediascanner/CMsgQuit.h"
#include "../mediascanner/CMsgResponse.h"
#include "../mediascanner/CMsgScanDir.h"
#include "../mediascanner/CMsgProgress.h"
#include "../mediascanner/CMsgFinished.h"
#include "../mediascanner/CMsgCollectionChanged.h"
#include "../mediascanner/CMsgOpenDb.h"


#include <string>
#include <vector>

#include <string.h>  // memcpy

CPPUNIT_TEST_SUITE_REGISTRATION( CSubProcessTest );

using namespace std;

CSubProcessTest::CSubProcessTest() {
	// TODO Auto-generated constructor stub

}

CSubProcessTest::~CSubProcessTest() {
	// TODO Auto-generated destructor stub
}

void CSubProcessTest::setUp() {
	m_mediaScanCtrl = new CMediaScannerCtrl(0);
	m_serBuf = 0;
}

void CSubProcessTest::tearDown() {
	delete m_mediaScanCtrl;
	if(m_serBuf != 0) {
		delete [] m_serBuf;
	}
}

void CSubProcessTest::start() {
	m_mediaScanCtrl->start();
	sleep(2);
	m_mediaScanCtrl->stop();
}

void CSubProcessTest::kill() {
	m_mediaScanCtrl->start();
	sleep(2);
	m_mediaScanCtrl->terminate();
}

void CSubProcessTest::serializeMsgQuit() {
	CMsgQuit quitmsg;

	int buffersize;
	char* buffer = quitmsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgQuit quitmsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( quitmsg == quitmsg2 );
}

void CSubProcessTest::serializeMsgResponse() {
	CMsgResponse respmsg(217, 5, "what went wrong?");

	int buffersize;
	char* buffer = respmsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgResponse respmsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( respmsg == respmsg2 );
}

void CSubProcessTest::serializeMsgOpenDB() {
	CMsgOpenDb odbmsg("/var/muroa/statedb.sqlite");

	int buffersize;
	char* buffer = odbmsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgOpenDb odbmsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( odbmsg == odbmsg2 );
}

void CSubProcessTest::serializeMsgScanDir() {
	CMsgScanDir sdmsg( "/usr/share/mp3" );

	int buffersize;
	char* buffer = sdmsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgScanDir sdmsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( sdmsg == sdmsg2 );
}

void CSubProcessTest::serializeMsgProgress() {
	CMsgProgress progmsg(12, 99);     // job No 12 is 99% finished

	int buffersize;
	char* buffer = progmsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgProgress progmsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( progmsg == progmsg2 );

}

void CSubProcessTest::serializeMsgFinished() {
	CMsgFinished finimsg(13);      // job Np 13 finished

	int buffersize;
	char* buffer = finimsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgFinished finimsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( finimsg == finimsg2 );

}

void CSubProcessTest::serializeMsgCollectionChanged() {
	CMsgCollectionChanged colchmsg(87, 1, 87);

	int buffersize;
	char* buffer = colchmsg.serialize(buffersize);
	m_serBuf = appendRandomData(buffer, buffersize);

	CMsgCollectionChanged colchmsg2(m_serBuf, buffersize);
	CPPUNIT_ASSERT( colchmsg == colchmsg2 );

}


char* CSubProcessTest::appendRandomData(const char* buffer, int& size ) {
	char* newBuffer = new char[size * 2];

	memcpy(newBuffer, buffer, size);
	for(int i = 0; i < size; i++) {
		newBuffer[size + 1] = rand();
	}

	size *= 2;
	return newBuffer;
}
