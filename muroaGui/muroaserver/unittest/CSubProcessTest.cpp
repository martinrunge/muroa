/*
 * CSubProcessTest.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CSubProcessTest.h"
#include "../CMediaScannerCtrl.h"

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
	m_mediaScanCtrl = new CMediaScannerCtrl();
}

void CSubProcessTest::tearDown() {
	delete m_mediaScanCtrl;

}

void CSubProcessTest::start() {
	m_mediaScanCtrl->start();
}

void CSubProcessTest::kill() {

}

