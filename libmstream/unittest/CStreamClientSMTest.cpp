/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamClientSMTest.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CStreamClientSMTest.h"

namespace muroa {

CPPUNIT_TEST_SUITE_REGISTRATION( CStreamClientSMTest );

CStreamClientSMTest::CStreamClientSMTest() : m_clnt_actions(0), m_clnt_sm(0) {
	// TODO Auto-generated constructor stub

}

CStreamClientSMTest::~CStreamClientSMTest() {
	// TODO Auto-generated destructor stub
}


void CStreamClientSMTest::setUp() {
	assert(m_clnt_actions == 0);
	m_clnt_actions = new CClientSMActions();

	assert(m_clnt_sm == 0);
    m_clnt_sm = new CStreamClientSM( m_clnt_actions);

}

void CStreamClientSMTest::tearDown() {
    delete m_clnt_sm;
    m_clnt_sm = 0;

    delete m_clnt_actions;
    m_clnt_actions = 0;
}

void CStreamClientSMTest::dontJoinTest() {
    VisitorBase vis;

	m_clnt_sm->start();
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

	m_clnt_sm->process_event( evClientState() );
	m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evRequestJoin() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evJoinRejected() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();
}

void CStreamClientSMTest::requestJoinTest() {
    VisitorBase vis;

	m_clnt_sm->start();
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

	m_clnt_sm->process_event( evClientState() );
	m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evRequestJoin() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evJoinRejected() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();
}


void CStreamClientSMTest::joinRejectedTest() {
    VisitorBase vis;

	m_clnt_sm->start();
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

	m_clnt_sm->process_event( evClientState() );
	m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evRequestJoin() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evJoinRejected() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();
}

void CStreamClientSMTest::joinAcceptedTest() {
    VisitorBase vis;
	m_clnt_sm->start();
    vis.printActiveStates();
    vis.clear();


	m_clnt_sm->process_event( evClientState() );
	m_clnt_sm->process_event( evRequestJoin() );
	m_clnt_sm->process_event( evJoinAccepted() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

	// in inner state joinedSession
	m_clnt_sm->process_event( evSessionState() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evSetVolume() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evSyncStream() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

    m_clnt_sm->process_event( evResetStream() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

	// an error happened
	m_clnt_sm->process_event( evError() );
    m_clnt_sm->visit_current_states(boost::ref(vis));
    vis.printActiveStates();
    vis.clear();

}

} /* namespace muroa */
