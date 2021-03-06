/*
 * CMuroaXmlRpcTest.cpp
 *
 *  Created on: 13 Aug 2011
 *      Author: martin
 */

#include "CMuroaXmlRpcTest.h"
#include "muroaxml/xmlCommands.h"
#include "CRpcDummy.h"

#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( CMuroaXmlRpcTest );

using namespace std;

CMuroaXmlRpcTest::CMuroaXmlRpcTest() : m_sessionName("Meine Über Session mit äöüß!"){
}

CMuroaXmlRpcTest::~CMuroaXmlRpcTest() {
}

void CMuroaXmlRpcTest::setUp() {
	m_rpc_in = new CRpcDummy();
	m_rpc_out = new CRpcDummy(m_rpc_in);
	m_rpc_out->open();
}

void CMuroaXmlRpcTest::tearDown() {
	m_rpc_out->close();
	delete m_rpc_out;
	delete m_rpc_in;
}

void CMuroaXmlRpcTest::listSessions() {
	// m_rpc_out->leaveSession();
	vector<string> sl;
	sl.push_back("default");
	sl.push_back("Special with whitspace");
	sl.push_back("Deutsch mit Umlauten äöüß");

	m_rpc_out->listSessions(sl);
	string last_cmd = m_rpc_in->getLastCmd();
	vector<string> recv_sl = m_rpc_in->getLastStringVec();

	CPPUNIT_ASSERT(last_cmd.compare(xmlCommands::sessionList) == 0);
	CPPUNIT_ASSERT(recv_sl == sl);
}

void CMuroaXmlRpcTest::joinSession() {

}

void CMuroaXmlRpcTest::play() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->play(1);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("play") == 0);
}

void CMuroaXmlRpcTest::pause() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->pause(2);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("pause") == 0);
}

void CMuroaXmlRpcTest::stop() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->stop(3);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("stop") == 0);
}

void CMuroaXmlRpcTest::prev() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->prev(4);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("prev") == 0);
}

void CMuroaXmlRpcTest::next() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->next(5);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("next") == 0);
}


void CMuroaXmlRpcTest::progress() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->progress(17, 89, 100);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("progress(17,89,100)") == 0);
}

void CMuroaXmlRpcTest::stateChanged() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->stateChanged(2);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("stateChanged(2)") == 0);
}

void CMuroaXmlRpcTest::error() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->error(17, -1, "does not match.");
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("error(17,-1,\"does not match.\")") == 0);
}

void CMuroaXmlRpcTest::getCollection() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->getCollection(7);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("getCollection(0)") == 0);

	m_rpc_out->getCollection(8, 17);
	last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("getCollection(17)") == 0);
}

void CMuroaXmlRpcTest::getPlaylist() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->getPlaylist(9);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("getPlaylist(0)") == 0);

	m_rpc_out->getPlaylist(10, 17);
	last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("getPlaylist(17)") == 0);
}

void CMuroaXmlRpcTest::getNextlist() {
	m_rpc_out->joinSession(m_sessionName);
	m_rpc_out->getNextlist(11);
	string last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("getNextlist(0)") == 0);

	m_rpc_out->getNextlist(12, 17);
	last_cmd = m_rpc_in->getLastCmd();

	CPPUNIT_ASSERT(last_cmd.compare("getNextlist(17)") == 0);
}

void CMuroaXmlRpcTest::collection() {
	m_rpc_out->joinSession(m_sessionName);
}

void CMuroaXmlRpcTest::playlist() {
	m_rpc_out->joinSession(m_sessionName);
}

void CMuroaXmlRpcTest::nextlist() {
	m_rpc_out->joinSession(m_sessionName);
}

void CMuroaXmlRpcTest::editCollection() {
	m_rpc_out->joinSession(m_sessionName);
}

void CMuroaXmlRpcTest::editPlaylist() {
	m_rpc_out->joinSession(m_sessionName);
}


void CMuroaXmlRpcTest::editNextlist() {
	m_rpc_out->joinSession(m_sessionName);
}

