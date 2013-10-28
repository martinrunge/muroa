/*
 * CUtilsTest.cpp
 *
 *  Created on: 20.09.2013
 *      Author: martin
 */

#include "CUtilsTest.h"

#include "CUtils.h"
#include <boost/filesystem.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( CUtilsTest );

using namespace boost::filesystem;

CUtilsTest::CUtilsTest() {
	// TODO Auto-generated constructor stub

}

CUtilsTest::~CUtilsTest() {
	// TODO Auto-generated destructor stub
}

void CUtilsTest::setUp() {

}

void CUtilsTest::tearDown() {

}

void CUtilsTest::expandvars() {
	path p("$HOME/.muroa/default");

	path p2 = CUtils::expandvars(p);

	CPPUNIT_ASSERT( true );

}
