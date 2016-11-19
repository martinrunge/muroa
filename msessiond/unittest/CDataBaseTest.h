/*
 * CDataBaseTest.h
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#ifndef CDATABASETEST_H_
#define CDATABASETEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <boost/asio.hpp>

namespace muroa {
class CStateDB;
class CSessionContaier;
class CSession;
class CMediaItemCategorizer;
}

class CStateDbUpdater;
class CFsScanner;
class CMediaScanner;

class CFakeMediaCollection;

class CDataBaseTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CDataBaseTest );
	  CPPUNIT_TEST( readMediaItem );
	  CPPUNIT_TEST( readGeneral );
	  CPPUNIT_TEST( selectColRevs );
	  //CPPUNIT_TEST( StateDbUpdater );
	  //CPPUNIT_TEST( noDBChange );
	  CPPUNIT_TEST( saveSession );
	  CPPUNIT_TEST( restoreSession );
	  CPPUNIT_TEST_SUITE_END();

public:
	CDataBaseTest();
	virtual ~CDataBaseTest();

	void setUp();
	void tearDown();

	void readMediaItem();
	void readGeneral();
	void selectColRevs();

	void saveSession();
	void restoreSession();

	muroa::CStateDB* m_stateDB;
	muroa::CMediaItemCategorizer* m_categorizer;
	CStateDbUpdater* m_stateDbUpdater;
	CFsScanner* m_fsScanner;
	CFakeMediaCollection* m_fakeCollection;
	CMediaScanner* m_mediaScanner;

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

	boost::asio::io_service m_io_service;
	muroa::CSessionContainer* m_sessionContainer;

	static muroa::CSession* m_session;
	static unsigned m_testHash;
	static int m_testHashPos;
};

#endif /* CDATABASETEST_H_ */
