/*
 * CTimeTest.h
 *
 *  Created on: 17 Sep 2010
 *      Author: martin
 */

#ifndef CTIMETEST_H_
#define CTIMETEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "../CTime.h"

class CTimeTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CTimeTest );
	  CPPUNIT_TEST( testSub );
	  CPPUNIT_TEST_SUITE_END();
public:
	CTimeTest();
	virtual ~CTimeTest();

	void setUp();
    void tearDown();

	void testSub();
};

#endif /* CTIMETEST_H_ */
