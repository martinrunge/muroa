/*
 * CSubProcessTest.h
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#ifndef CSUBPROCESSTEST_H_
#define CSUBPROCESSTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class CMediaScannerCtrl;


class CSubProcessTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CSubProcessTest );
	  CPPUNIT_TEST( start );
	  CPPUNIT_TEST( kill );
	  CPPUNIT_TEST( serializeMsgQuit );
	  CPPUNIT_TEST( serializeMsgResponse );
	  CPPUNIT_TEST( serializeMsgOpenDB );
	  CPPUNIT_TEST( serializeMsgScanDir );
	  CPPUNIT_TEST( serializeMsgProgress );
	  CPPUNIT_TEST( serializeMsgFinished );
	  CPPUNIT_TEST( serializeMsgCollectionChanged );
	  CPPUNIT_TEST_SUITE_END();

public:
	CSubProcessTest();
	virtual ~CSubProcessTest();

	void setUp();
	void tearDown();

	void start();
	void kill();

	void serializeMsgQuit();
	void serializeMsgResponse();
	void serializeMsgOpenDB();
	void serializeMsgScanDir();
	void serializeMsgProgress();
	void serializeMsgFinished();
	void serializeMsgCollectionChanged();

private:
	CMediaScannerCtrl* m_mediaScanCtrl;
	char* appendRandomData(const char* buffer, int& size );
	char* m_serBuf;
};

#endif /* CSUBPROCESSTEST_H_ */
