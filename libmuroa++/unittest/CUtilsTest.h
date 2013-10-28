/*
 * CUtilsTest.h
 *
 *  Created on: 20.09.2013
 *      Author: martin
 */

#ifndef CUTILSTEST_H_
#define CUTILSTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class CUtilsTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CUtilsTest );
	  CPPUNIT_TEST( expandvars );
	  CPPUNIT_TEST_SUITE_END();
public:
	CUtilsTest();
	virtual ~CUtilsTest();

	void setUp();
    void tearDown();
	void expandvars();

};

#endif /* CUTILSTEST_H_ */
