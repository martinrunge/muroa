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

namespace muroa {
  class CCategoryItem;
  class CRootItem;
}

class CCollectionTest: public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CCollectionTest );
	  CPPUNIT_TEST( replaceTabs );
	  CPPUNIT_TEST( construct );
	  CPPUNIT_TEST( serializeMedia1 );
	  CPPUNIT_TEST( serializeMedia2 );
	  CPPUNIT_TEST( serializeMedia3 );
	  CPPUNIT_TEST( deserializeMedia1 );
	  CPPUNIT_TEST( deserializeMedia2 );
	  CPPUNIT_TEST( deserializeMedia3 );
	  CPPUNIT_TEST( deserializeMedia4 );
	  CPPUNIT_TEST( serializePlaylist );
	  CPPUNIT_TEST( deserializePlaylist );
	  CPPUNIT_TEST( serializeNextlist );
	  CPPUNIT_TEST( deserializeNextlist );
	  CPPUNIT_TEST( serializeStreamClient );
	  CPPUNIT_TEST( deserializeStreamClient );
	  CPPUNIT_TEST( serializeCategory );
	  CPPUNIT_TEST( traverse );
	  CPPUNIT_TEST( serialize );
	  CPPUNIT_TEST( deserialize );
      CPPUNIT_TEST( copyConstruct );
	  CPPUNIT_TEST_SUITE_END();

public:
	CCollectionTest();
	virtual ~CCollectionTest();

	void setUp();
    void tearDown();

    void replaceTabs();
	void construct();
	void traverse();
	void serializeMedia1();
	void serializeMedia2();
	void serializeMedia3();
    void deserializeMedia1();
    void deserializeMedia2();
    void deserializeMedia3();
    void deserializeMedia4();
	void serializePlaylist();
	void deserializePlaylist();
	void serializePlaylist2();
	void deserializePlaylist2();
	void serializeNextlist();
	void deserializeNextlist();
	void serializeNextlist2();
	void deserializeNextlist2();
	void serializeStreamClient();
	void deserializeStreamClient();
	void serializeStreamClient2();
	void deserializeStreamClient2();
	void serializeCategory();
	void serialize();
    void deserialize();
    void copyConstruct();

private:
    muroa::CRootItem* m_root;

    muroa::CRootItem* prepareFakeCollection(int numArtist, int numAlbum, int numTitle);

};

#endif /* CCOLLECTIONTEST_H_ */
