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
	  CPPUNIT_TEST( serializeClientState );
	  CPPUNIT_TEST( serializeRequestJoin );
	  CPPUNIT_TEST( serializeJoinAccepted );
	  CPPUNIT_TEST( serializeJoinRejected );
	  CPPUNIT_TEST( serializeLeave );

	  CPPUNIT_TEST( serializeGetSessionState );
	  CPPUNIT_TEST( serializeSessionState );

	  CPPUNIT_TEST( serializeResetStream );
	  CPPUNIT_TEST( serializeSyncStream );
	  CPPUNIT_TEST( serializeSetVolume );

	  CPPUNIT_TEST( serializeError );
	  CPPUNIT_TEST( serializeAck );
	  CPPUNIT_TEST_SUITE_END();

public:
	CCmdStreamTests();
	virtual ~CCmdStreamTests();

	void setUp();
    void tearDown();

    void serializeClientState();

    void serializeRequestJoin();
    void serializeJoinAccepted();
    void serializeJoinRejected();
    void serializeLeave();

    void serializeGetSessionState();
    void serializeSessionState();

	void serializeResetStream();
	void serializeSyncStream();
	void serializeSetVolume();

    void serializeError();
    void serializeAck();

private:

	CStreamCtrlRpcDummy* m_rpc_s;
	CStreamCtrlRpcDummy* m_rpc_r;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_UNITTEST_CCMDSTREAMTESTS_H_ */
