/***************************************************************************
 *
 *   CCollectionTest.h
 *
 *   This file is part of libmuroa++                                  *
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

#ifndef CCOLLECTIONTEST_H_
#define CCOLLECTIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class CCategoryItem;
class CRootItem;

class CCollectionTest: public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CCollectionTest );
	  CPPUNIT_TEST( replaceTabs );
	  CPPUNIT_TEST( construct );
	  CPPUNIT_TEST( serializeMedia );
	  CPPUNIT_TEST( deserializeMedia );
	  CPPUNIT_TEST( serializePlaylist );
	  CPPUNIT_TEST( deserializePlaylist );
	  CPPUNIT_TEST( serializeNextlist );
	  CPPUNIT_TEST( deserializeNextlist );
	  CPPUNIT_TEST( serializeCategory );
	  CPPUNIT_TEST( traverse );
	  CPPUNIT_TEST( serialize );
	  CPPUNIT_TEST( deserialize );
	  CPPUNIT_TEST_SUITE_END();

public:
	CCollectionTest();
	virtual ~CCollectionTest();

	void setUp();
    void tearDown();

    void replaceTabs();
	void construct();
	void traverse();
	void serializeMedia();
    void deserializeMedia();
	void serializePlaylist();
	void deserializePlaylist();
	void serializeNextlist();
	void deserializeNextlist();
	void serializeCategory();
	void serialize();
    void deserialize();

private:
    CRootItem* m_root;

    CRootItem* prepareFakeCollection(int numArtist, int numAlbum, int numTitle);

};

#endif /* CCOLLECTIONTEST_H_ */
