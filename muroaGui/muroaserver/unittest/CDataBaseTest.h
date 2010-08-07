/*
 * CDataBaseTest.h
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#ifndef CDATABASETEST_H_
#define CDATABASETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class CStateDB;
class CCollectionUpdater;

class CDataBaseTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CDataBaseTest );
	  CPPUNIT_TEST( testDB );
	  CPPUNIT_TEST_SUITE_END();

public:
	CDataBaseTest();
	virtual ~CDataBaseTest();

	void setUp();
	void tearDown();

	void testDB();

	CStateDB* m_stateDB;
    CCollectionUpdater* m_colUpdater;

};

#endif /* CDATABASETEST_H_ */
