/*
 * CMuroaXmlRpcTest.h
 *
 *  Created on: 13 Aug 2011
 *      Author: martin
 */

#ifndef CMUROAXMLRPCTEST_H_
#define CMUROAXMLRPCTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class CMuroaXml;

class CMuroaXmlRpcTest: public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CMuroaXmlRpcTest );
	  CPPUNIT_TEST( join );
	  CPPUNIT_TEST( leave );
	  CPPUNIT_TEST( play );
	  CPPUNIT_TEST( stop );
	  CPPUNIT_TEST( prev );
	  CPPUNIT_TEST( next );
	  CPPUNIT_TEST_SUITE_END();

public:
	CMuroaXmlRpcTest();
	~CMuroaXmlRpcTest();

	void setUp();
    void tearDown();

	void join();
	void leave();
	void play();
	void stop();
	void prev();
	void next();

	CMuroaXml *m_rpc_in;
	CMuroaXml *m_rpc_out;
};

#endif /* CMUROAXMLRPCTEST_H_ */
