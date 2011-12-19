/*
 * CIteratorTest.cpp
 *
 *  Created on: 11 Dec 2011
 *      Author: martin
 */

#include "CIteratorTest.h"

#include "CRootItem.h"
#include "CUtils.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CIteratorTest );

CIteratorTest::CIteratorTest() {

}

CIteratorTest::~CIteratorTest() {

}

void CIteratorTest::setUp() {
	m_rootItem = new CRootItem;

	m_orig = CUtils::file2string( "unittest/testcases/CIteratorTest_forward1/testcol.txt" );
	m_rootItem->deserialize(m_orig);

}

void CIteratorTest::tearDown() {
	delete m_rootItem;
}

void CIteratorTest::forward() {
	CRootItem::iterator it = m_rootItem->begin();

	string output;
	for(; it != m_rootItem->end(); it++ ) {
		output += (*it)->getText();
	}
    cerr << endl << "---" << endl << output << endl << "----" << endl;


	CPPUNIT_ASSERT( output.compare(m_orig) == 0 );
}

void CIteratorTest::backward() {
	CRootItem::iterator it = m_rootItem->end();
//	for(; it != m_rootItem->begin(); --it ) {
//
//	}
}
