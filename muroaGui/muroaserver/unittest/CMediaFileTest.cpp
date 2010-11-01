/*
 * CMediaFileTest.cpp
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#include "CMediaFileTest.h"

#include "../mediascanner/CFsScanner.h"

#include <iostream>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CMediaFileTest );

CMediaFileTest::CMediaFileTest() {
	// TODO Auto-generated constructor stub

}

CMediaFileTest::~CMediaFileTest() {
	// TODO Auto-generated destructor stub
}


void CMediaFileTest::setUp() {
	m_fsScanner = new CFsScanner();
}

void CMediaFileTest::tearDown() {
	delete m_fsScanner;
}

void CMediaFileTest::recurseDir() {

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	vector<CMediaItem*>* collection = 0;

	m_fsScanner->setFileTypes(types);
	collection = m_fsScanner->walkTree("/home/martin");

	CPPUNIT_ASSERT( collection != 0 );
	CPPUNIT_ASSERT( collection->size() > 0 );

}
