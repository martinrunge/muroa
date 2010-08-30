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
	  CPPUNIT_TEST( readGeneral );
	  CPPUNIT_TEST( selectColRevs );
	  CPPUNIT_TEST_SUITE_END();

public:
	CDataBaseTest();
	virtual ~CDataBaseTest();

	void setUp();
	void tearDown();

	void testDB();
	void readGeneral();
	void selectColRevs();

	CStateDB* m_stateDB;
    CCollectionUpdater* m_colUpdater;

private:
	const std::string colMinRevVal;
	const std::string colMaxRevVal;

	const std::string plMinRevVal;
	const std::string plMaxRevVal;

	const std::string nlMinRevVal;
	const std::string nlMaxRevVal;


};

#endif /* CDATABASETEST_H_ */
