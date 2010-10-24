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
	  CPPUNIT_TEST_SUITE_END();
public:
	CSubProcessTest();
	virtual ~CSubProcessTest();

	void setUp();
	void tearDown();

	void start();
	void kill();

private:
	CMediaScannerCtrl* m_mediaScanCtrl;

};

#endif /* CSUBPROCESSTEST_H_ */
