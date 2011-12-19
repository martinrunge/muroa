/*
 * CIteratorTest.h
 *
 *  Created on: 11 Dec 2011
 *      Author: martin
 */

#ifndef CITERATORTEST_H_
#define CITERATORTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <string>

class CRootItem;

class CIteratorTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CIteratorTest );
	  CPPUNIT_TEST( forward );
	  CPPUNIT_TEST( backward );
	  CPPUNIT_TEST_SUITE_END();

public:
	CIteratorTest();
	virtual ~CIteratorTest();

	void setUp();
    void tearDown();

    void forward();
    void backward();

private:
	CRootItem* m_rootItem;
	std::string m_orig;
};

#endif /* CITERATORTEST_H_ */
