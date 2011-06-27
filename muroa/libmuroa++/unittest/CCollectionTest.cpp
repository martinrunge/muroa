/***************************************************************************
 *
 *   CCollectionTest.cpp
 *
 *   This file is part of libmuroa++                                   *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "CCollectionTest.h"
#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CRootItem.h"

#include <cmath>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CCollectionTest );

CCollectionTest::CCollectionTest() {

}

CCollectionTest::~CCollectionTest() {

}

void CCollectionTest::setUp() {
	m_root = new CRootItem();
}

void CCollectionTest::tearDown() {
	delete m_root;
}


void CCollectionTest::construct() {
	cerr << "CCollectionTest::construct()" << endl;
	CCategoryItem *item;


	item = m_root->addCategory("stufe1");
	item = m_root->addCategory("stufe2", item);
	item = m_root->addCategory("stufe3", item);

	CMediaItem *mItem = m_root->addMediaItem("/path/to/file.mp3\tTest Artist 0\tTest Album 0	Test Title 9\t2008\t90\t3560093084", item);
}

void CCollectionTest::serializeMedia() {
	CMediaItem *mItem = new CMediaItem(m_root, "/path/to/file.mp3\tTest Artist 0\tTest Album 0	Test Title 9\t2008\t90\t3560093084", 0);
	string result = mItem->serialize();

	stringstream ss;
	ss << mItem->getText();
	string expected = ss.str();

	delete mItem;
	CPPUNIT_ASSERT(result.compare(expected) == 0 );
}

void CCollectionTest::deserializeMedia() {
	CMediaItem *mItem = new CMediaItem(m_root, "/path/to/file.mp3\tTest Artist 0\tTest Album 0	Test Title 9\t2008\t90\t2665035088", 0);
	size_t hashval = mItem->getHash();
	delete mItem;
	CPPUNIT_ASSERT( hashval == 2665035088UL );
}


void CCollectionTest::serializeCategory() {
	CCategoryItem* base = new CCategoryItem(m_root);
	CCategoryItem *cItem = new CCategoryItem(m_root, "Category1", base);
	CMediaItem *mItem = new CMediaItem(m_root, "Titel1", cItem);
	string result = base->serialize();

	stringstream ss;
	ss << mItem->getText();
	string expected = ss.str();
	CPPUNIT_ASSERT(result.compare(expected) == 0 );
}


void CCollectionTest::serialize() {
	CCategoryItem *item;
	CCategoryItem* base = new CCategoryItem(m_root);

	item = new CCategoryItem(m_root, "stufe1", base);
	item = new CCategoryItem(m_root, "stufe2", item);
	item = new CCategoryItem(m_root, "stufe3", item);

	CMediaItem *mItem = new CMediaItem(m_root, "Titel1", item);

	string result = base->serialize();

	stringstream ss;
	ss << mItem->getText();
	string expected = ss.str();
	CPPUNIT_ASSERT(result.compare(expected) == 0 );
}

void CCollectionTest::deserialize() {
	CRootItem* rItem = prepareFakeCollection(10, 10, 10);

	string serialisation = rItem->serialize();

	cerr << serialisation << endl << endl;

	CRootItem* newRoot = new CRootItem( );
	newRoot->deserialize(serialisation);

	string newserialisation = newRoot->serialize();
	cerr << newserialisation << endl << endl;

	CPPUNIT_ASSERT( newserialisation.compare(serialisation) == 0 );
	CPPUNIT_ASSERT( (*newRoot) == (*rItem) );
}


void CCollectionTest::traverse() {

}

void CCollectionTest::replaceTabs() {
	CMediaItem *mItem = new CMediaItem(m_root, 0);
	mItem->setArtist("Tabula\tRasa\t");
	mItem->setAlbum("On The \t way to \tabs");
	mItem->setTitle("\tTab\tTab\t");

	string name = mItem->getName();
	CPPUNIT_ASSERT( name.compare("") == 0 );

	string artist = mItem->getArtist();
	CPPUNIT_ASSERT( artist.compare("Tabula Rasa") == 0 );

	string album = mItem->getAlbum();
	CPPUNIT_ASSERT( album.compare("On The   way to  abs") == 0 );

	string title = mItem->getTitle();
	CPPUNIT_ASSERT( title.compare("Tab Tab") == 0 );
}




CRootItem* CCollectionTest::prepareFakeCollection(int numArtist, int numAlbum, int numTitle) {

	CRootItem* rItem = new CRootItem();

	for(int i=0; i < numArtist; i++) {
		stringstream artist_ss;
		artist_ss << "Test Artist " << setw( floor(log10(numArtist)) ) << i;
		CCategoryItem* artist = rItem->addCategory(artist_ss.str());

		for(int j=0; j < numAlbum; j++) {
			stringstream album_ss;
			album_ss << "Test Album " << setw( floor(log10(numAlbum)) ) << j;
			CCategoryItem* album = rItem->addCategory(album_ss.str(), artist);

			for(int k=0; k < numArtist; k++) {
				stringstream title_ss;
				title_ss << "Test Title " << setw( floor(log10(numTitle)) ) << k;
				CMediaItem* title = rItem->addMediaItem(title_ss.str(), album);
				title->setFilename("/path/to/file.mp3");
				title->setArtist(artist_ss.str());
				title->setAlbum(album_ss.str());
				title->setTitle(title_ss.str());
				title->setYear(2008);
				title->setDuration( 10*k );

			}
		}
	}

	return rItem;
}


