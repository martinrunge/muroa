/*
 * CMediaScannerTest.cpp
 *
 *  Created on: 16 Nov 2010
 *      Author: martin
 */

#include "CMediaScannerTest.h"

#include "../mediascanner/CMediaScanner.h"
#include "../mediascanner/CFsScanner.h"

#include "../mediascanner/CMsgScanDir.h"
#include "../mediascanner/CMsgQuit.h"

#include <iostream>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CMediaScannerTest );

CMediaScannerTest::CMediaScannerTest() {

}

CMediaScannerTest::~CMediaScannerTest() {
	if(m_thread.joinable()) {
		m_thread.join();
	}
}


void CMediaScannerTest::setUp() {
	m_media_scanner = new CMediaScanner();
	m_fs_scanner = new CFsScanner(m_media_scanner);
}

void CMediaScannerTest::tearDown() {
	delete m_media_scanner;
	delete m_fs_scanner;

	if(m_thread.joinable()) {
		m_thread.join();
	}
}

void CMediaScannerTest::testEventLoop() {

	quitIn(30);
	m_fs_scanner->scanDir("/home/martin");
	m_media_scanner->run();

	CPPUNIT_ASSERT( 1 );
}


void CMediaScannerTest::testScanDirEvent() {
	CMsgScanDir* msg = new CMsgScanDir("/home/martin");

	quitIn(30);
	m_media_scanner->postEvent(msg);
	m_media_scanner->run();


}

void CMediaScannerTest::quitIn(int sec) {
	m_thread = std::thread(&CMediaScannerTest::quitThread, this, sec );
}

void CMediaScannerTest::quitThread(int sec) {
	sleep(sec);
	CMsgQuit* quitMsg = new CMsgQuit();
	m_media_scanner->postEvent(quitMsg);
}
