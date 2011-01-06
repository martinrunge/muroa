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


#include <string>
#include <vector>

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
}

void CSubProcessTest::tearDown() {
	delete m_mediaScanCtrl;

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

	CMsgQuit quitmsg2(buffer, buffersize);
	CPPUNIT_ASSERT( quitmsg == quitmsg2 );
}

void CSubProcessTest::serializeMsgResponse() {
	CMsgResponse respmsg(217, 5, "what went wrong?");

	int buffersize;
	char* buffer = respmsg.serialize(buffersize);

	CMsgResponse respmsg2(buffer, buffersize);
	CPPUNIT_ASSERT( respmsg == respmsg2 );
}

void CSubProcessTest::serializeMsgScanDir() {
	CMsgScanDir sdmsg( "/usr/share/mp3" );

	int buffersize;
	char* buffer = sdmsg.serialize(buffersize);

	CMsgScanDir sdmsg2(buffer, buffersize);
	CPPUNIT_ASSERT( sdmsg == sdmsg2 );
}

void CSubProcessTest::serializeMsgProgress() {

}
