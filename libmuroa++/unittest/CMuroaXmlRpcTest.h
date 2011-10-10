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

class CRpcDummy;


class CMuroaXmlRpcTest: public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CMuroaXmlRpcTest );
	  CPPUNIT_TEST( play );
	  CPPUNIT_TEST( pause );
	  CPPUNIT_TEST( stop );
	  CPPUNIT_TEST( prev );
	  CPPUNIT_TEST( next );

	  CPPUNIT_TEST( progress );
	  CPPUNIT_TEST( stateChanged );
	  CPPUNIT_TEST( error );

	  CPPUNIT_TEST(getCollection);
	  CPPUNIT_TEST(getPlaylist);
	  CPPUNIT_TEST(getNextlist);

	  CPPUNIT_TEST(collection);
	  CPPUNIT_TEST(playlist);
	  CPPUNIT_TEST(nextlist);

	  CPPUNIT_TEST(editCollection);
	  CPPUNIT_TEST(editPlaylist);
	  CPPUNIT_TEST(editNextlist);

	  CPPUNIT_TEST_SUITE_END();

public:
	CMuroaXmlRpcTest();
	~CMuroaXmlRpcTest();

	void setUp();
    void tearDown();

	void play();
	void pause();
	void stop();
	void prev();
	void next();

	void progress();
	void stateChanged();
	void error();

	void getCollection();
	void getPlaylist();
	void getNextlist();

	void collection();
	void playlist();
	void nextlist();

	void editCollection();
	void editPlaylist();
	void editNextlist();

private:
	CRpcDummy *m_rpc_in;
	CRpcDummy *m_rpc_out;

	const uint32_t m_sessionID;
};

#endif /* CMUROAXMLRPCTEST_H_ */
