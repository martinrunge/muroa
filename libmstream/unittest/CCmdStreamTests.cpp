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

void CCmdStreamTests::serializeError() {
	m_rpc_s->error(1, 1, "error message");

	string lastCmd = m_rpc_r->getLastCmdInternal();
	CPPUNIT_ASSERT(lastCmd.compare("onError(1, 1, 'error message')") == 0);

}

void muroa::CCmdStreamTests::serializeJoinSession() {

	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");
	uint32_t cmdID = 2;
	string sesion_name("default session");

	m_rpc_s->joinSession(cmdID, sesion_name, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdIdInternal();
	string lastCmd = m_rpc_r->getLastCmdInternal();
	boost::asio::ip::address received_address = m_rpc_r->getLastIpAddrInternal();

	CPPUNIT_ASSERT( lastCmdID == cmdID );
	CPPUNIT_ASSERT( lastCmd.compare( sesion_name ) == 0);
	CPPUNIT_ASSERT(received_address == server_addr);

}

void muroa::CCmdStreamTests::serializeTakeFromSession() {

	uint32_t cmdID = 3;
	string sesion_name("default session");
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	m_rpc_s->takeFromSession(cmdID, sesion_name, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdIdInternal();
	string lastCmd = m_rpc_r->getLastCmdInternal();
	boost::asio::ip::address received_address = m_rpc_r->getLastIpAddrInternal();

	CPPUNIT_ASSERT( lastCmdID == cmdID );
	CPPUNIT_ASSERT( lastCmd.compare( sesion_name ) == 0);
	CPPUNIT_ASSERT(received_address == server_addr);

}


void CCmdStreamTests::serializeStreamReset() {
	uint32_t joinSessionCmdID = 4;
	string joinSessionName = "default session";
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	m_rpc_s->joinSession(joinSessionCmdID, joinSessionName, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdIdInternal();
	if(lastCmdID == joinSessionCmdID) {
		m_rpc_r->ack(lastCmdID);
	}

	uint32_t ackCmd = m_rpc_s->getLastAck();
	CPPUNIT_ASSERT( ackCmd == joinSessionCmdID );
	if(ackCmd == joinSessionCmdID) {
		m_rpc_s->setState(IStreamCtrl::SESSION_STATE);
		m_rpc_r->setState(IStreamCtrl::SESSION_STATE);
	}

	uint32_t cmdID = 5;
	uint32_t ssrc = 324784;
	m_rpc_s->resetStream(cmdID, ssrc);

	lastCmdID = m_rpc_r->getLastCmdIdInternal();
	uint32_t lastSsrc = m_rpc_r->getLastSsrcInternal();

	string lastCmd = m_rpc_r->getLastCmdInternal();
	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( ssrc == lastSsrc );
	CPPUNIT_ASSERT(lastCmd.compare("onResetStream(5, 324784)") == 0);
}


void CCmdStreamTests::testGetSetTimeSrv() {
	uint32_t joinSessionCmdID = 6;
	string joinSessionName = "default session";
	boost::asio::ip::address server_addr = address::from_string("192.168.1.77");

	m_rpc_s->joinSession(joinSessionCmdID, joinSessionName, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdIdInternal();
	if(lastCmdID == joinSessionCmdID) {
		m_rpc_r->ack(lastCmdID);
	}

	uint32_t ackCmd = m_rpc_s->getLastAck();
	CPPUNIT_ASSERT( ackCmd == joinSessionCmdID );
	if(ackCmd == joinSessionCmdID) {
		m_rpc_s->setState(IStreamCtrl::SESSION_STATE);
		m_rpc_r->setState(IStreamCtrl::SESSION_STATE);
	}

	// now part of session
    // set a time serv addr and port in m_rpc_r and then let m_rpc_s get it.

	uint32_t cmdID = 7;
	uint32_t port = 5779;
	m_rpc_r->setTimesrvIpAddrInternal(server_addr);
	m_rpc_r->setTimesrvPortInternal(port);

	m_rpc_s->getTimeSrv(cmdID);

	// m_rpc_r will send the resonse to m_rpc_s

	lastCmdID = m_rpc_s->getLastCmdIdInternal();
	boost::asio::ip::address timesrvAddr = m_rpc_s->getTimesrvIpAddrInternal();
	uint32_t timesrvPort = m_rpc_s->getTimesrvPortInternal();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( port == timesrvPort );
	CPPUNIT_ASSERT( server_addr == timesrvAddr );

	cmdID++;
	server_addr = address::from_string("192.168.2.78");
	port = 5998;

	m_rpc_s->setTimeSrv(cmdID, server_addr, port);

	lastCmdID = m_rpc_r->getLastCmdIdInternal();
	timesrvAddr = m_rpc_r->getTimesrvIpAddrInternal();
	timesrvPort = m_rpc_r->getTimesrvPortInternal();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( port == timesrvPort );
	CPPUNIT_ASSERT( server_addr == timesrvAddr );

	cmdID = 8;
	m_rpc_s->getTimeSrv(cmdID);
	lastCmdID = m_rpc_s->getLastCmdIdInternal();
	timesrvAddr = m_rpc_s->getTimesrvIpAddrInternal();
	timesrvPort = m_rpc_s->getTimesrvPortInternal();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( port == timesrvPort );
	CPPUNIT_ASSERT( server_addr == timesrvAddr );
}

void CCmdStreamTests::testGetSetStreamTimeBase() {
	uint32_t joinSessionCmdID = 9;
	string joinSessionName = "default session";
	boost::asio::ip::address server_addr = address::from_string("FE80::0202:B3FF:FE1E:8329");

	m_rpc_s->joinSession(joinSessionCmdID, joinSessionName, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdIdInternal();
	if(lastCmdID == joinSessionCmdID) {
		m_rpc_r->ack(lastCmdID);
	}

	uint32_t ackCmd = m_rpc_s->getLastAck();
	CPPUNIT_ASSERT( ackCmd == joinSessionCmdID );
	if(ackCmd == joinSessionCmdID) {
		m_rpc_s->setState(IStreamCtrl::SESSION_STATE);
		m_rpc_r->setState(IStreamCtrl::SESSION_STATE);
	}

	// now part of session
	uint32_t cmdID = 10;
	uint32_t ssrc = 46437;
	uint64_t pts = 324345478593LL;
	uint64_t rtp_ts = 38650943785934LL;

	m_rpc_r->setTimebaseSsrcInternal(ssrc);
	m_rpc_r->setTimebasePtsInternal(pts);
	m_rpc_r->setTimebaseRtpTsInternal(rtp_ts);

	m_rpc_s->getStreamTimeBase(cmdID, ssrc);

	// receiver should respond with "setStreamTimeBase" with same cmdID

	lastCmdID = m_rpc_s->getLastCmdIdInternal();
	uint32_t last_ssrc = m_rpc_s->getTimebaseSsrcInternal();
	uint64_t last_pts = m_rpc_s->getTimebasePtsInternal();
	uint64_t last_rtp_ts = m_rpc_s->getTimebaseRtpTsInternal();


	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( ssrc == last_ssrc );
	CPPUNIT_ASSERT( pts == last_pts );
	CPPUNIT_ASSERT( rtp_ts == last_rtp_ts );

	// set a new time base

	cmdID++;
	ssrc = 46437;
	pts = 55887345478545LL;
	rtp_ts = 88933386509434LL;

	m_rpc_s->setStreamTimeBase(cmdID, ssrc, rtp_ts, pts);

	// receiver should respond with "ack" with same cmdID

	// uint32_t lastAck = m_rpc_s->getLastAck();
	lastCmdID = m_rpc_r->getLastCmdIdInternal();
	last_ssrc = m_rpc_r->getTimebaseSsrcInternal();
	last_pts = m_rpc_r->getTimebasePtsInternal();
	last_rtp_ts = m_rpc_r->getTimebaseRtpTsInternal();

	CPPUNIT_ASSERT( ssrc == last_ssrc );
	CPPUNIT_ASSERT( pts == last_pts );
	CPPUNIT_ASSERT( rtp_ts == last_rtp_ts );

	if(ssrc == last_ssrc && pts == last_pts && rtp_ts == last_rtp_ts) {
		m_rpc_r->ack(lastCmdID);
	}


}

void CCmdStreamTests::testGetSetRtpPort() {
	joinSession();

	uint32_t cmdID = 10;
	uint32_t rtp_port = 5437;

	m_rpc_r->setRtpPortInternal(rtp_port);

	m_rpc_s->getRTPPort(cmdID);

	// receiver should respond with "setRTPPort" with same cmdID

	uint32_t lastCmdID = m_rpc_s->getLastCmdIdInternal();
	uint32_t last_rtp_port = m_rpc_s->getRtpPortInternal();


	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( rtp_port == last_rtp_port );

	// set a new time base

	cmdID++;
	rtp_port = 7345;

	m_rpc_s->setRTPPort(cmdID, rtp_port);

	// receiver should respond with "ack" with same cmdID

	uint32_t lastAck = m_rpc_s->getLastAck();

	lastCmdID = m_rpc_r->getLastCmdIdInternal();
	last_rtp_port = m_rpc_r->getRtpPortInternal();

	CPPUNIT_ASSERT( lastCmdID == cmdID );
	CPPUNIT_ASSERT( rtp_port == last_rtp_port );

}

void CCmdStreamTests::testGetSetVolume() {
	joinSession();

	uint32_t cmdID = 10;
	uint32_t volume = 75;

	m_rpc_r->setVolPercentInternal(volume);

	m_rpc_s->getVolume(cmdID);

	// receiver should respond with "setRTPPort" with same cmdID

	uint32_t lastCmdID = m_rpc_s->getLastCmdIdInternal();
	uint32_t last_volume = m_rpc_s->getVolPercentInternal();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( volume == last_volume );

	// set a new volume

	cmdID++;
	volume = 90;

	m_rpc_s->setVolume(cmdID, volume);

	lastCmdID = m_rpc_r->getLastCmdIdInternal();

	if(lastCmdID == cmdID) {
		m_rpc_r->ack(lastCmdID);
	}
	// receiver should respond with "ack" with same cmdID

	uint32_t lastAck = m_rpc_s->getLastAck();
	last_volume = m_rpc_r->getVolPercentInternal();

	CPPUNIT_ASSERT( lastAck == cmdID );
	CPPUNIT_ASSERT( last_volume == volume );

	leaveSession();
}

void CCmdStreamTests::testGetSetLeaveMCastGrp() {
	joinSession();

	uint32_t cmdID = 10;
	boost::asio::ip::address mcast_grp = address::from_string("224.100.100.39");

	m_rpc_r->setMCastGrpInternal(mcast_grp);

	m_rpc_s->getMCastGrp(cmdID);

	// receiver should respond with "setRTPPort" with same cmdID

	uint32_t lastCmdID = m_rpc_s->getLastCmdIdInternal();
	boost::asio::ip::address last_mcast_grp = m_rpc_s->getMCastGrpInternal();

	CPPUNIT_ASSERT( cmdID == lastCmdID );
	CPPUNIT_ASSERT( last_mcast_grp == mcast_grp );

	// set a new mcast group

	cmdID++;
	mcast_grp = address::from_string("224.101.101.40");

	m_rpc_s->joinMCastGrp(cmdID, mcast_grp);

	lastCmdID = m_rpc_r->getLastCmdIdInternal();

	if(lastCmdID == cmdID) {
		m_rpc_r->ack(lastCmdID);
	}
	// receiver should respond with "ack" with same cmdID

	uint32_t lastAck = m_rpc_s->getLastAck();
	last_mcast_grp = m_rpc_r->getMcastGrpInternal();

	CPPUNIT_ASSERT( lastAck == cmdID );
	CPPUNIT_ASSERT( last_mcast_grp == mcast_grp );

	leaveSession();
}

void CCmdStreamTests::joinSession(std::string name) {
	uint32_t joinSessionCmdID = 19;
	string joinSessionName = name;
	boost::asio::ip::address server_addr = address::from_string("FE80::0202:B3FF:FE1E:8329");

	m_rpc_s->joinSession(joinSessionCmdID, joinSessionName, server_addr);

	uint32_t lastCmdID = m_rpc_r->getLastCmdIdInternal();
	if(lastCmdID == joinSessionCmdID) {
		m_rpc_r->ack(lastCmdID);
	}

	uint32_t ackCmd = m_rpc_s->getLastAck();
	CPPUNIT_ASSERT( ackCmd == joinSessionCmdID );
	if(ackCmd == joinSessionCmdID) {
		m_rpc_s->setState(IStreamCtrl::SESSION_STATE);
		m_rpc_r->setState(IStreamCtrl::SESSION_STATE);
	}
}

void CCmdStreamTests::leaveSession() {
	m_rpc_s->joinSessionLeave();

	m_rpc_s->setState(IStreamCtrl::ROOT_STATE);
	m_rpc_r->setState(IStreamCtrl::ROOT_STATE);
}

} /* namespace muroa */
