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

#ifndef LIBMSTREAM_UNITTEST_CCMDSTREAMTESTS_H_
#define LIBMSTREAM_UNITTEST_CCMDSTREAMTESTS_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "CStreamCtrlRpcDummy.h"

namespace muroa {

class CCmdStreamTests : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CCmdStreamTests );
	  CPPUNIT_TEST( serializeStreamReset );
	  CPPUNIT_TEST( serializeError );
	  CPPUNIT_TEST( serializeJoinSession );
	  CPPUNIT_TEST( serializeTakeFromSession );
	  CPPUNIT_TEST( testGetSetTimeSrv );
	  CPPUNIT_TEST( testGetSetStreamTimeBase );
	  CPPUNIT_TEST( testGetSetRtpPort );
	  CPPUNIT_TEST( testGetSetVolume );
	  CPPUNIT_TEST( testGetSetLeaveMCastGrp );
	  CPPUNIT_TEST_SUITE_END();

public:
	CCmdStreamTests();
	virtual ~CCmdStreamTests();

	void setUp();
    void tearDown();

    void serializeError();

    void serializeJoinSession();
    void serializeTakeFromSession();

	void serializeStreamReset();

	void testGetSetTimeSrv();
	void testGetSetStreamTimeBase();
	void testGetSetRtpPort();
	void testGetSetVolume();

	void testGetSetLeaveMCastGrp();



private:
	CStreamCtrlRpcDummy* m_rpc_s;
	CStreamCtrlRpcDummy* m_rpc_r;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_UNITTEST_CCMDSTREAMTESTS_H_ */
