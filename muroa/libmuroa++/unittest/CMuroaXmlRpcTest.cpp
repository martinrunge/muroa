/*
 * CMuroaXmlRpcTest.cpp
 *
 *  Created on: 13 Aug 2011
 *      Author: martin
 */

#include "CMuroaXmlRpcTest.h"

#include "muroaxml/CMuroaXml.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CMuroaXmlRpcTest );

CMuroaXmlRpcTest::CMuroaXmlRpcTest() {
}

CMuroaXmlRpcTest::~CMuroaXmlRpcTest() {
}

void CMuroaXmlRpcTest::setUp() {
	m_rpc_in = new CMuroaXml();
	m_rpc_out = new CMuroaXml();
}

void CMuroaXmlRpcTest::tearDown() {
	delete m_rpc_out;
	delete m_rpc_in;
}

void CMuroaXmlRpcTest::join() {
}

void CMuroaXmlRpcTest::leave() {
}

void CMuroaXmlRpcTest::play() {
}

void CMuroaXmlRpcTest::stop() {
}

void CMuroaXmlRpcTest::prev() {
}

void CMuroaXmlRpcTest::next() {
}

