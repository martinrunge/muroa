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
class CStateDbUpdater;
class CCollectionUpdater;
class CSession;
class CFakeMediaCollection;

class CDataBaseTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CDataBaseTest );
	  CPPUNIT_TEST( testDB );
	  CPPUNIT_TEST( readGeneral );
	  CPPUNIT_TEST( selectColRevs );
	  CPPUNIT_TEST( StateDbUpdater );
	  CPPUNIT_TEST( noDBChange );
	  CPPUNIT_TEST( saveSession );
	  CPPUNIT_TEST( restoreSession );
	  CPPUNIT_TEST_SUITE_END();

public:
	CDataBaseTest();
	virtual ~CDataBaseTest();

	void setUp();
	void tearDown();

	void testDB();
	void readGeneral();
	void selectColRevs();

	void StateDbUpdater();
	void noDBChange();

	void saveSession();
	void restoreSession();

	CStateDB* m_stateDB;
	CStateDbUpdater* m_stateDbUpdater;
	CCollectionUpdater* m_colUpdater;
	CFakeMediaCollection* m_fakeCollection;

private:
	const std::string colMinRevVal;
	const std::string colMaxRevVal;

	const std::string plMinRevVal;
	const std::string plMaxRevVal;

	const std::string nlMinRevVal;
	const std::string nlMaxRevVal;

	void prepareSession();
	void preparePlaylist();
	void prepareNextlist();

	static CSession* m_session;
	static unsigned m_testHash;
	static int m_testHashPos;
};

#endif /* CDATABASETEST_H_ */
