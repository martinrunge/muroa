/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCmdStreamTests.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CCmdStreamTests.h"

#include "cmds/CmdStreamConnect.h"

#include <string>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio::ip;

namespace muroa {

CPPUNIT_TEST_SUITE_REGISTRATION( CCmdStreamTests );

CCmdStreamTests::CCmdStreamTests() {
	// TODO Auto-generated constructor stub

}

CCmdStreamTests::~CCmdStreamTests() {
	// TODO Auto-generated destructor stub
}

void CCmdStreamTests::setUp() {
	m_rpc_r = new CStreamCtrlRpcDummy();
	m_rpc_s = new CStreamCtrlRpcDummy();

	m_rpc_s->setReceiver(m_rpc_r);
	m_rpc_r->setReceiver(m_rpc_s);

    m_rpc_r->setup();
    m_rpc_s->setup();
    // uint32_t cmdID = m_rpc_->open();
}

void CCmdStreamTests::tearDown() {
	// m_rpc_s->close();
	m_rpc_r->shutdown();
	m_rpc_s->shutdown();
	delete m_rpc_s;
	delete m_rpc_r;
}


void CCmdStreamTests::serializeClientState() {
	evClientState ev_cst;
	ev_cst.m_cmd_id = 14;
	ev_cst.m_member_of_session = "Meine Ätzende Umlaut Session";
	ev_cst.m_current_volume = 78;
	ev_cst.m_session_srv = address::from_string("192.168.1.77");
	m_rpc_s->sendEvent(&ev_cst);

	const CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	const evClientState* r_cst = dynamic_cast<const evClientState*>(cmdBase);
	CPPUNIT_ASSERT( ev_cst == *r_cst );

}

void muroa::CCmdStreamTests::serializeRequestJoin() {

	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");
	uint32_t cmdID = 2;
	string session_name("default session");

	evRequestJoin rjoin;
	rjoin.m_cmd_id = cmdID;
	rjoin.m_session_name = session_name;
	rjoin.m_mcast_addr = server_addr;
	m_rpc_s->sendEvent(&rjoin);

	const CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	const evRequestJoin* r_jreq = dynamic_cast<const evRequestJoin*>(cmdBase);
	CPPUNIT_ASSERT( rjoin == *r_jreq );

}

void muroa::CCmdStreamTests::serializeJoinAccepted() {

	evJoinAccepted joinAcked;
	joinAcked.m_cmd_id = 38;
	joinAcked.m_former_session = "Die andere Session";
	m_rpc_s->sendEvent(&joinAcked);

	const CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	const evJoinAccepted* r_jAck = dynamic_cast<const evJoinAccepted*>(cmdBase);
	CPPUNIT_ASSERT( joinAcked == *r_jAck );
}

void muroa::CCmdStreamTests::serializeJoinRejected() {

	evJoinRejected rjoin;
	rjoin.m_cmd_id = 765;
	rjoin.m_message = "No Way!!!";
	rjoin.m_owner_session = "My other session";
	m_rpc_s->sendEvent(&rjoin);

	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evJoinRejected* r_jrej = dynamic_cast<evJoinRejected*>(cmdBase);
	CPPUNIT_ASSERT( rjoin == *r_jrej );

}

void muroa::CCmdStreamTests::serializeLeave() {

	evLeave leave;
	leave.m_cmd_id = 658;
	leave.m_triggered_by_session = "Other session";
	m_rpc_s->sendEvent(&leave);

	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evLeave* r_leave = dynamic_cast<evLeave*>(cmdBase);
	CPPUNIT_ASSERT( leave == *r_leave );
}



void CCmdStreamTests::serializeGetSessionState() {
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	evGetSessionState ev_sst;
	ev_sst.m_cmd_id = 76;
	ev_sst.m_session_name = "my session äöü mit Umlaut";
	m_rpc_s->sendEvent(&ev_sst);


	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evGetSessionState* r_sst = dynamic_cast<evGetSessionState*>(cmdBase);
	CPPUNIT_ASSERT( ev_sst == *r_sst );
}

void CCmdStreamTests::serializeSessionState() {

	evSessionState ev_sst;
	ev_sst.m_cmd_id = 76;
	ev_sst.m_session_name = "my session äöü mit Umlaut";
	ev_sst.m_mcast_addr = address::from_string("192.168.1.77");
	ev_sst.m_rtp_unicast_port = 5554;
	ev_sst.m_timesrv_port = 6667;
	m_rpc_s->sendEvent(&ev_sst);


	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evSessionState* r_sst = dynamic_cast<evSessionState*>(cmdBase);
	CPPUNIT_ASSERT( ev_sst == *r_sst );
}

void CCmdStreamTests::serializeResetStream() {
	uint32_t cmdID = 4;
	uint32_t ssrc = 674826482;
	uint64_t pts = 3498775234;
	uint32_t rtp_ts = 74398;
	string joinSessionName = "default session";
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	evResetStream srst;
	srst.m_cmd_id = cmdID;
	srst.m_ssrc = ssrc;
	srst.m_media_clock_pts = pts;
	srst.m_rtp_ts = rtp_ts;
	m_rpc_s->sendEvent(&srst);


	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evResetStream* r_srst = dynamic_cast<evResetStream*>(cmdBase);
	CPPUNIT_ASSERT( srst == *r_srst );
}

void CCmdStreamTests::serializeSyncStream() {

	evSyncStream sst;
	sst.m_cmd_id = 8374;
	sst.m_ssrc = 248392472;
	sst.m_media_clock_pts = 753759374;
	sst.m_rtp_ts = 25234;
	m_rpc_s->sendEvent(&sst);


	const CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	const evSyncStream* r_sst = dynamic_cast<const evSyncStream*>(cmdBase);
	CPPUNIT_ASSERT( sst == *r_sst );
}

void CCmdStreamTests::serializeSetVolume() {

	evSetVolume sv;
	sv.m_cmd_id = 8374;
	sv.m_ssrc = 248392472;
	sv.m_volume = 89;
	m_rpc_s->sendEvent(&sv);


	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evSetVolume* r_sv = dynamic_cast<evSetVolume*>(cmdBase);
	CPPUNIT_ASSERT( sv == *r_sv );
}

void CCmdStreamTests::serializeError() {
	evError err;
	err.m_cmd_id = 17;
	err.m_error_msg = "error message";
	m_rpc_s->sendEvent(&err);

	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evError* r_err = dynamic_cast<evError*>(cmdBase);
	CPPUNIT_ASSERT( err == *r_err );

}

void CCmdStreamTests::serializeAck() {
	evAck ack;
	ack.m_cmd_id = 99;
	m_rpc_s->sendEvent(&ack);

	CmdStreamBase* cmdBase = m_rpc_r->getLastEv();
	CPPUNIT_ASSERT( cmdBase != 0 );

	evAck* r_ack = dynamic_cast<evAck*>(cmdBase);
	CPPUNIT_ASSERT( ack == *r_ack );
}

} /* namespace muroa */
