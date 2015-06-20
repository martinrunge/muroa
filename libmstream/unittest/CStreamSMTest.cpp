/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamSMTest.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CStreamSMTest.h"

namespace muroa {

CPPUNIT_TEST_SUITE_REGISTRATION( CStreamSMTest );

CStreamSMTest::CStreamSMTest() : m_srv_sm(0) {
	// TODO Auto-generated constructor stub

}

CStreamSMTest::~CStreamSMTest() {
	// TODO Auto-generated destructor stub
}


void CStreamSMTest::setUp() {
	assert(m_srv_sm == 0);
    m_srv_sm = new CStreamSrvSM();
}

void CStreamSMTest::tearDown() {
    delete m_srv_sm;
    m_srv_sm = 0;
}

void CStreamSMTest::joinRejectedTest() {
    VisitorBase vis;

	m_srv_sm->start();
    m_srv_sm->visit_current_states(boost::ref(vis));

	m_srv_sm->process_event( evClientState() );
	m_srv_sm->visit_current_states(boost::ref(vis));

    m_srv_sm->process_event( evRequestJoin() );
    m_srv_sm->visit_current_states(boost::ref(vis));

    m_srv_sm->process_event( evJoinRejected() );
    m_srv_sm->visit_current_states(boost::ref(vis));
}

void CStreamSMTest::joinAcceptedTest() {
    VisitorBase vis;
	m_srv_sm->start(); m_srv_sm->pstate();

	m_srv_sm->process_event( evClientState() );
	m_srv_sm->process_event( evRequestJoin() );
	m_srv_sm->process_event( evJoinAccepted() );

	// in inner state joinedSession
	m_srv_sm->process_event( evSessionState() );
    m_srv_sm->visit_current_states(boost::ref(vis));

    m_srv_sm->process_event( evSetVolume() );
    m_srv_sm->visit_current_states(boost::ref(vis));

    m_srv_sm->process_event( evSyncStream() );
    m_srv_sm->visit_current_states(boost::ref(vis));

    m_srv_sm->process_event( evResetStream() );
    m_srv_sm->visit_current_states(boost::ref(vis));

	// an error happened
	m_srv_sm->process_event( evError() );
    m_srv_sm->visit_current_states(boost::ref(vis));

}

} /* namespace muroa */
