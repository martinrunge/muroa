/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCmdStreamTests.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef UNITTEST_CCMDSTREAMTESTS_H_
#define UNITTEST_CCMDSTREAMTESTS_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace muroa {

class CCmdStreamTests : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CCmdStreamTests );
	  CPPUNIT_TEST( serializeStreamReset );
	  CPPUNIT_TEST( serializeStreamConnectIPv4 );
	  CPPUNIT_TEST( serializeStreamConnectIPv6 );
	  CPPUNIT_TEST( deserializeStreamConnectIPv6 );
	  CPPUNIT_TEST( deserializeStreamConnectIPv4 );
	  CPPUNIT_TEST_SUITE_END();
public:
	CCmdStreamTests();
	virtual ~CCmdStreamTests();

	void setUp();
    void tearDown();

	void serializeStreamReset();

	void serializeStreamConnectIPv4();
	void serializeStreamConnectIPv6();
	void deserializeStreamConnectIPv4();
	void deserializeStreamConnectIPv6();
};

} /* namespace muroa */

#endif /* UNITTEST_CCMDSTREAMTESTS_H_ */
