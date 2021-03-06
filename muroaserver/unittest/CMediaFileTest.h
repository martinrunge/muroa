/*
 * CMediaFileTest.h
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#ifndef CMEDIAFILETEST_H_
#define CMEDIAFILETEST_H_

#include <cppunit/extensions/HelperMacros.h>

class CMediaScanner;
class CFsScanner;

class CMediaFileTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CMediaFileTest );
	  CPPUNIT_TEST( buildFakeCollection );
	  CPPUNIT_TEST( iterDir );
	  CPPUNIT_TEST( recurseDir );
	  CPPUNIT_TEST( recurseDirBFS );
	  CPPUNIT_TEST_SUITE_END();
public:
	CMediaFileTest();
	virtual ~CMediaFileTest();

    void setUp();
    void tearDown();

    void buildFakeCollection();
    void iterDir();
    void recurseDir();
    void recurseDirBFS();

private:
    CFsScanner* m_fsScanner;
    CMediaScanner* m_mediaScanner;
};

#endif /* CMEDIAFILETEST_H_ */
