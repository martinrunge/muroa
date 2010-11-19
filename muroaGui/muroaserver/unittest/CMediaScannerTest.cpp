/*
 * CMediaScannerTest.cpp
 *
 *  Created on: 16 Nov 2010
 *      Author: martin
 */

#include "CMediaScannerTest.h"

#include "../mediascanner/CMediaScanner.h"
#include "../mediascanner/CFsScanner.h"

#include <iostream>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CMediaScannerTest );


CMediaScannerTest::CMediaScannerTest() {

}

CMediaScannerTest::~CMediaScannerTest() {
}


void CMediaScannerTest::setUp() {
	m_media_scanner = new CMediaScanner();
	m_fs_scanner = new CFsScanner(m_media_scanner);
}

void CMediaScannerTest::tearDown() {
	delete m_media_scanner;
	delete m_fs_scanner;
}

void CMediaScannerTest::testEventLoop() {
	m_fs_scanner->scanDir("/home/martin");
	m_media_scanner->run();

	CPPUNIT_ASSERT( 1 );
}
