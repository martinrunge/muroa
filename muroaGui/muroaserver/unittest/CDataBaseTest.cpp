/*
 * CDataBaseTest.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CDataBaseTest.h"
#include "../CStateDB.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CDataBaseTest );

CDataBaseTest::CDataBaseTest() {
}

CDataBaseTest::~CDataBaseTest() {
}

void CDataBaseTest::setUp()
{
	m_stateDB = new CStateDB( "TestDB.sqlite" );
}

void CDataBaseTest::tearDown()
{
	delete m_stateDB;
}

void CDataBaseTest::testDB()
{
	int rc = 0;
	m_stateDB->open();

	m_stateDB->close();

	CPPUNIT_ASSERT( rc == 0 );

}
