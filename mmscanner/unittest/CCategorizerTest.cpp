/*
 * CCategorizerTest.cpp
 *
 *  Created on: 9 Jan 2012
 *      Author: martin
 */

#include "CCategorizerTest.h"

#include <CSession.h>
#include <CMediaItemCategorizer.h>

#include <CRootItem.h>
#include <CMediaItem.h>

CPPUNIT_TEST_SUITE_REGISTRATION( CCategorizerTest );

using namespace std;


CCategorizerTest::CCategorizerTest() {
}


CCategorizerTest::~CCategorizerTest() {
}


void CCategorizerTest::setUp() {
	m_session = new muroa::CSession("name");
	m_categorizer = new muroa::CMediaItemCategorizer();
}



void CCategorizerTest::tearDown() {
	delete m_session;
	delete m_categorizer;
}



void CCategorizerTest::ByArtistAlbum() {
	CMediaItem mi1, mi2, mi3;

	std::vector<CMediaItem*> mItems;
	mi1.setAlbum("Album1");
	mi1.setArtist("Artist1");
	mi1.setTitle("Title1");
	mItems.push_back(&mi1);

	mi2.setAlbum("Album1");
	mi2.setArtist("Artist1");
	mi2.setTitle("Title2");
	mItems.push_back(&mi2);

	mi3.setAlbum("Album1");
	mi3.setArtist("Artist1");
	mi3.setTitle("Title3");
	mItems.push_back(&mi3);

	CRootItem* ri = m_categorizer->categorize(&mItems);
}



void CCategorizerTest::ByFileSystem() {
}



