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

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	m_fs_scanner->setFileTypes( types );
}

void CMediaScannerTest::tearDown() {
	delete m_media_scanner;
	delete m_fs_scanner;

	if(m_thread.joinable()) {
		m_thread.join();
	}
}

void CMediaScannerTest::testEventLoop() {

	m_fs_scanner->scanDir("/home/martin");
	run();
	sleep(30);
	CMsgQuit* quitMsg = new CMsgQuit();
	m_media_scanner->postEvent(quitMsg);
	join();
	CPPUNIT_ASSERT( 1 );
}


void CMediaScannerTest::testScanDirEvent() {
	CMsgScanDir* msg = new CMsgScanDir("/home/martin");

	run();

	m_media_scanner->postEvent(msg);
	sleep(30);
	CMsgQuit* quitMsg = new CMsgQuit();
	m_media_scanner->postEvent(quitMsg);
	join();

}

void CMediaScannerTest::run() {
	m_thread = std::thread(&CMediaScanner::run, m_media_scanner);
}

void CMediaScannerTest::join() {
	if(m_thread.joinable()) {
		m_thread.join();
	}
}
