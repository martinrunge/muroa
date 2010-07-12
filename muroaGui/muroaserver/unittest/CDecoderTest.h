/*
 * CDecoderTest.h
 *
 *  Created on: 11 Jul 2010
 *      Author: martin
 */

#ifndef CDECODERTEST_H_
#define CDECODERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class CDecoderTest : public CppUnit::TestFixture{
	  CPPUNIT_TEST_SUITE( CDecoderTest );
	  CPPUNIT_TEST( test );
	  CPPUNIT_TEST_SUITE_END();
public:
	CDecoderTest();
	virtual ~CDecoderTest();

  void setUp();
  void tearDown();

  void test();

};

#endif /* CDECODERTEST_H_ */
