/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamSMTest.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef UNITTEST_CSTREAMCLIENTSMTEST_H_
#define UNITTEST_CSTREAMCLIENTSMTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "CClientSMActions.h"
#include "CStreamClientSM.h"


namespace muroa {

class IClientSMActions;

class CStreamClientSMTest  : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CStreamClientSMTest );
	  CPPUNIT_TEST( dontJoinTest );
	  CPPUNIT_TEST( requestJoinTest );
	  CPPUNIT_TEST( joinAcceptedTest );
	  CPPUNIT_TEST( joinRejectedTest );
	  CPPUNIT_TEST_SUITE_END();

public:
	CStreamClientSMTest();
	virtual ~CStreamClientSMTest();

	void setUp();
    void tearDown();

    void dontJoinTest();
    void requestJoinTest();
    void joinAcceptedTest();
    void joinRejectedTest();



private:
    CStreamClientSM *m_clnt_sm;
    IClientSMActions *m_clnt_actions;

};

} /* namespace muroa */

#endif /* UNITTEST_CSTREAMCLIENTSMTEST_H_ */
