/*
 * CMediaScannerTest.h
 *
 *  Created on: 16 Nov 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNERTEST_H_
#define CMEDIASCANNERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include <thread>

class CMediaScanner;
class CFsScanner;

class CMediaScannerTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CMediaScannerTest );
	  CPPUNIT_TEST( testEventLoop );
	  CPPUNIT_TEST( testScanDirEvent );
	  CPPUNIT_TEST_SUITE_END();
public:
	CMediaScannerTest();
	virtual ~CMediaScannerTest();

    void setUp();
    void tearDown();

	void testEventLoop();
	void testScanDirEvent();

private:
	void run();
	void join();

	CMediaScanner* m_media_scanner;
	CFsScanner* m_fs_scanner;

	std::thread m_thread;
};

#endif /* CMEDIASCANNERTEST_H_ */
