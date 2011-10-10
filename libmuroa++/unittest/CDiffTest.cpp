/***************************************************************************
 *
 *   CDiffTest.cpp
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

#include "CDiffTest.h"
#include "CRootItem.h"

#include "CUtils.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CDiffTest );

CDiffTest::CDiffTest() {

}

CDiffTest::~CDiffTest() {

}

void CDiffTest::setUp() {
	m_rootItem = new CRootItem;
}

void CDiffTest::tearDown() {
	delete m_rootItem;
}

void CDiffTest::diff() {
	string orig = CUtils::file2string( "unittest/testcases/CDiffTest_diff/origcollection.txt" );
	string modified = CUtils::file2string( "unittest/testcases/CDiffTest_diff/modcollection.txt" );

	string difference = m_diff.diff(orig, modified);
	cerr << "### CDiffTest::diff ###" << endl << difference << endl << "### end of CDiffTest::diff ###" << endl;

	string expected_diff = CUtils::file2string( "unittest/testcases/CDiffTest_diff/diff.txt" );

	CPPUNIT_ASSERT(expected_diff.compare(difference) == 0);
}

void CDiffTest::patch() {
	string orig = CUtils::file2string( "unittest/testcases/CDiffTest_patch/origcollection.txt" );
	string diff = CUtils::file2string( "unittest/testcases/CDiffTest_patch/diff.txt" );

	string modified = m_diff.patch(orig, diff);
	cerr << "### CDiffTest::patch ###" << endl << modified << endl << "### end of CDiffTest::patch ###" << endl;

	string expected_modified = CUtils::file2string( "unittest/testcases/CDiffTest_patch/modcollection.txt" );

	CPPUNIT_ASSERT(expected_modified.compare(modified) == 0);
}

