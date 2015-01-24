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

    m_rpc_r->open();
    m_rpc_s->open();
    // uint32_t cmdID = m_rpc_->open();
}

void CCmdStreamTests::tearDown() {
	// m_rpc_s->close();
	m_rpc_r->close();
	m_rpc_s->close();
	delete m_rpc_s;
	delete m_rpc_r;
}

void CCmdStreamTests::serializeError() {
	m_rpc_s->error(1, 1, "error message");

	string lastCmd = m_rpc_r->getLastCmd();
	CPPUNIT_ASSERT(lastCmd.compare("onError(1, 1, 'error message')") == 0);

}

void muroa::CCmdStreamTests::serializeJoinSession() {

	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");
	uint32_t cmdID = 2;
	string sesion_name("default session");

	m_rpc_s->joinSession(cmdID, sesion_name, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdId();
	string lastCmd = m_rpc_r->getLastCmd();
	boost::asio::ip::address received_address = m_rpc_r->getLastIpAddr();

	CPPUNIT_ASSERT( lastCmdID == cmdID );
	CPPUNIT_ASSERT( lastCmd.compare( sesion_name ) == 0);
	CPPUNIT_ASSERT(received_address == server_addr);

}

void muroa::CCmdStreamTests::serializeTakeFromSession() {

	uint32_t cmdID = 3;
	string sesion_name("default session");
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	m_rpc_s->takeFromSession(cmdID, sesion_name, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdId();
	string lastCmd = m_rpc_r->getLastCmd();
	boost::asio::ip::address received_address = m_rpc_r->getLastIpAddr();

	CPPUNIT_ASSERT( lastCmdID == cmdID );
	CPPUNIT_ASSERT( lastCmd.compare( sesion_name ) == 0);
	CPPUNIT_ASSERT(received_address == server_addr);

}


void CCmdStreamTests::serializeStreamReset() {
	uint32_t joinSessionCmdID = 4;
	string joinSessionName = "default session";
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	m_rpc_s->joinSession(joinSessionCmdID, joinSessionName, server_addr);

	uint32_t cmdID = 5;
	uint32_t ssrc = 324784;
	m_rpc_s->resetStream(cmdID, ssrc);

	uint32_t lastCmdID = m_rpc_r->getLastCmdId();
	uint32_t lastSsrc = m_rpc_r->getLastSsrc();

	string lastCmd = m_rpc_r->getLastCmd();
	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( ssrc == lastSsrc );
	CPPUNIT_ASSERT(lastCmd.compare("onResetStream(5, 324784)") == 0);
}


void CCmdStreamTests::testGetSetTimeSrv() {
	uint32_t joinSessionCmdID = 6;
	string joinSessionName = "default session";
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	m_rpc_s->joinSession(joinSessionCmdID, joinSessionName, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdId();
	if(lastCmdID == joinSessionCmdID) {
		m_rpc_r->ack(lastCmdID);
	}

	uint32_t ackCmd = m_rpc_s->getLastAck();

	CPPUNIT_ASSERT( ackCmd == joinSessionCmdID );

	uint32_t cmdID = 7;
	uint32_t port = 5779;
	m_rpc_s->setTimeSrv(cmdID, server_addr, port);

	lastCmdID = m_rpc_r->getLastCmdId();
	boost::asio::ip::address timesrvAddr = m_rpc_r->getTimesrvIpAddr();
	uint32_t timesrvPort = m_rpc_r->getTimesrvPort();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( port == timesrvPort );
	CPPUNIT_ASSERT( server_addr == timesrvAddr );

	cmdID = 8;
	m_rpc_s->getTimeSrv(cmdID);
	lastCmdID = m_rpc_s->getLastCmdId();
	timesrvAddr = m_rpc_s->getTimesrvIpAddr();
	timesrvPort = m_rpc_s->getTimesrvPort();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( port == timesrvPort );
	CPPUNIT_ASSERT( server_addr == timesrvAddr );

}

void CCmdStreamTests::testGetSetStreamTimeBase() {
}

void CCmdStreamTests::testGetSetRtpPort() {
}

void CCmdStreamTests::testGetSetVolume() {
}

void CCmdStreamTests::testGetSetLeaveMCastGrp() {
}

} /* namespace muroa */
