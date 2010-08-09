/*
 * CDataBaseTest.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CDataBaseTest.h"
#include "../CStateDB.h"
#include "CCollectionUpdater.h"

#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( CDataBaseTest );

using namespace std;

CDataBaseTest::CDataBaseTest() {
}

CDataBaseTest::~CDataBaseTest() {
}

void CDataBaseTest::setUp()
{
	m_stateDB = new CStateDB( "TestDB.sqlite" );
    m_colUpdater = new CCollectionUpdater();

}

void CDataBaseTest::tearDown()
{
	delete m_colUpdater;
	delete m_stateDB;
}

void CDataBaseTest::testDB()
{
	int rc = 0;
	m_stateDB->open();

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	CCollection<CCollectionItem>* collection = 0;

	m_colUpdater->setFileTypes(types);
	collection = m_colUpdater->walkTree("/home/martin");

	m_stateDB->updateCollectionDB(collection);

	m_stateDB->getSongIdByHash( 90039379 );

	m_stateDB->close();

	CPPUNIT_ASSERT( rc == 0 );

}
