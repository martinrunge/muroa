/*
 * CMediaFileTest.cpp
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#include "CMediaFileTest.h"

#include "../mediascanner/CMediaScanner.h"
#include "../mediascanner/CFsScanner.h"
#include "../mediascanner/CMediaItem.h"

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
	m_mediaScanner = new CMediaScanner(0);
	m_fsScanner = new CFsScanner(m_mediaScanner);
}

void CMediaFileTest::tearDown() {
	delete m_fsScanner;
	delete m_mediaScanner;
}

void CMediaFileTest::recurseDir() {

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	vector<CMediaItem*>* collection = 0;

	m_fsScanner->setFileTypes(types);
	m_fsScanner->scanDir("/home/martin");
	// wait here until scan finished
	std::vector<CMediaItem*> *scanres = m_fsScanner->finishScan();

	CPPUNIT_ASSERT( scanres != 0 );
	CPPUNIT_ASSERT( scanres->size() > 0 );

	for(std::vector<CMediaItem*>::iterator it = scanres->begin(); it != scanres->end(); it++ ) {
		CMediaItem* item = *it;
		delete item;
	}

	delete scanres;
}
