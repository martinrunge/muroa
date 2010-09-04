/*
 * CDataBaseTest.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CDataBaseTest.h"
#include "../CStateDB.h"
#include "CCollectionUpdater.h"
#include "../CSession.h"

#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( CDataBaseTest );

using namespace std;

CDataBaseTest::CDataBaseTest() : colMinRevVal("1"),
                                 colMaxRevVal("101"),
                                 plMinRevVal("2"),
                                 plMaxRevVal("102"),
                                 nlMinRevVal("3"),
                                 nlMaxRevVal("103")
{
}

CDataBaseTest::~CDataBaseTest() {
}

void CDataBaseTest::setUp()
{
	m_stateDB = new CStateDB( "TestDB.sqlite" );
    m_colUpdater = new CCollectionUpdater();

}

void CDataBaseTest::tearDown()
{
	delete m_colUpdater;
	delete m_stateDB;
}

void CDataBaseTest::testDB()
{
	int rc = 0;
	m_stateDB->open();

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	CCollection<CCollectionItem>* collection = 0;

	m_colUpdater->setFileTypes(types);
	collection = m_colUpdater->walkTree("/home/martin");
	CSession session;
	session.addCollectionRev(collection);
	session.addCollectionRev(collection);

	m_stateDB->updateCollectionDB(&session);

	m_stateDB->getCollectionItemByHash( 90039379 );

	m_stateDB->setValue("CollectionRevMin", colMinRevVal);
	m_stateDB->setValue("CollectionRevMax", colMaxRevVal);

	m_stateDB->setValue("PlaylistRevMin", plMinRevVal);
	m_stateDB->setValue("PlaylistRevMax", plMaxRevVal);

	m_stateDB->setValue("NextlistRevMin", nlMinRevVal);
	m_stateDB->setValue("NextlistRevMax", nlMaxRevVal);


	CCollectionItem* writtenItem = collection->getItem(1);
	CCollectionItem* readItem = m_stateDB->getCollectionItemByPos(1, 0);
	if(writtenItem->getHash() != readItem->getHash() ) rc = 1;

	m_stateDB->close();

	CPPUNIT_ASSERT( rc == 0 );
}


void CDataBaseTest::readGeneral() {
	m_stateDB->open();

	std::string colMinRev = m_stateDB->getValue("CollectionRevMin");
	std::string colMaxRev = m_stateDB->getValue("CollectionRevMax");

	std::string plMinRev = m_stateDB->getValue("PlaylistRevMin");
	std::string plMaxRev = m_stateDB->getValue("PlaylistRevMax");

	std::string nlMinRev = m_stateDB->getValue("NextlistRevMin");
	std::string nlMaxRev = m_stateDB->getValue("NextlistRevMax");

	cerr << "Collection revs: [" << colMinRev << ".." << colMaxRev << "]" << endl;
	cerr << "Playlist revs:   [" << plMinRev << ".." << plMaxRev << "]" << endl;
	cerr << "Nextlist revs:   [" << nlMinRev << ".." << nlMaxRev << "]" << endl;

	m_stateDB->close();
	CPPUNIT_ASSERT( colMinRev.compare(colMinRevVal) == 0 &&
                    colMaxRev.compare(colMaxRevVal) == 0 &&
                    plMinRev.compare(plMinRevVal) == 0 &&
                    plMaxRev.compare(plMaxRevVal) == 0 &&
                    nlMinRev.compare(nlMinRevVal) == 0 &&
                    nlMaxRev.compare(nlMaxRevVal) == 0 );
}


void CDataBaseTest::selectColRevs() {
	m_stateDB->open();

	int rowID = m_stateDB->rowIDofColRevEntry(1, 20722355, 0);

	cerr << "RowID of (1, 20722355, 0): "<< rowID << endl;

	m_stateDB->close();
	CPPUNIT_ASSERT( rowID != 0 );

}
