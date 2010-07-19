/*
 * CMediaFileTest.cpp
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#include "CMediaFileTest.h"

#include "CCollectionUpdater.h"

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
    m_colUpdater = new CCollectionUpdater();
}

void CMediaFileTest::tearDown() {
	delete m_colUpdater;
}

void CMediaFileTest::recurseDir() {

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	m_colUpdater->setFileTypes(types);
	m_colUpdater->walkTree("/home/martin");

	CPPUNIT_FAIL( "not implemented" );
}
