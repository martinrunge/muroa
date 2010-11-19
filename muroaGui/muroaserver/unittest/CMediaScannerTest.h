/*
 * CMediaScannerTest.h
 *
 *  Created on: 16 Nov 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNERTEST_H_
#define CMEDIASCANNERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class CMediaScanner;
class CFsScanner;

class CMediaScannerTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CMediaScannerTest );
	  CPPUNIT_TEST( testEventLoop );
	  CPPUNIT_TEST_SUITE_END();
public:
	CMediaScannerTest();
	virtual ~CMediaScannerTest();

    void setUp();
    void tearDown();

	void testEventLoop();

private:
	CMediaScanner* m_media_scanner;
	CFsScanner* m_fs_scanner;
};

#endif /* CMEDIASCANNERTEST_H_ */
