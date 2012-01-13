/*
 * CCategorizerTest.h
 *
 *  Created on: 9 Jan 2012
 *      Author: martin
 */

#ifndef CCATEGORIZERTEST_H_
#define CCATEGORIZERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace muroa {
class CSession;
class CMediaItemCategorizer;
}

class CCategorizerTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CCategorizerTest );
	  CPPUNIT_TEST( ByArtistAlbum );
	  CPPUNIT_TEST( ByFileSystem );
	  CPPUNIT_TEST_SUITE_END();

public:
	CCategorizerTest();
	virtual ~CCategorizerTest();

	void setUp();
	void tearDown();

	void ByArtistAlbum();
	void ByFileSystem();

private:
	muroa::CSession *m_session;
	muroa::CMediaItemCategorizer *m_categorizer;


};

#endif /* CCATEGORIZERTEST_H_ */
