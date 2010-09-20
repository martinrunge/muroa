/*
 * CTimeTest.cpp
 *
 *  Created on: 17 Sep 2010
 *      Author: martin
 */

#include "CTimeTest.h"
#include <iostream>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CTimeTest );

CTimeTest::CTimeTest() {
	// TODO Auto-generated constructor stub

}

CTimeTest::~CTimeTest() {
	// TODO Auto-generated destructor stub
}

void CTimeTest::setUp() {

}

void CTimeTest::tearDown() {

}

void CTimeTest::testSub() {
	CTime starttime = CTime::now();

	usleep(1000);
	CTime endtime = CTime::now();
	CDuration duration = endtime - starttime;
	cerr << "Duration (1ms): " << duration.sec() << endl;

	usleep(1000);
	endtime = CTime::now();
	duration = endtime - starttime;
	cerr << "Duration (2ms): " << duration.sec() << endl;

	usleep(1000);
	endtime = CTime::now();
	duration = endtime - starttime;
	cerr << "Duration (3ms): " << duration.sec() << endl;

	usleep(1000);
	endtime = CTime::now();
	duration = endtime - starttime;
	cerr << "Duration (4ms): " << duration.sec() << endl;

	usleep(1000);
	endtime = CTime::now();
	duration = endtime - starttime;
	cerr << "Duration (5ms): " << duration.sec() << endl;

	CPPUNIT_ASSERT( true );
}



