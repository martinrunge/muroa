/*
 * CDataBaseTest.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CDataBaseTest.h"
#include "CFakeMediaCollection.h"
#include <CStateDB.h>
#include "../../mmscanner/CStateDbUpdater.h"
#include "../../mmscanner/CFsScanner.h"
#include "../../mmscanner/CMediaScanner.h"
#include "../../mmscanner/CMediaItemCategorizer.h"
#include <CSession.h>
#include <CRootItem.h>
#include <CMediaItem.h>

#include <boost/filesystem.hpp>

#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( CDataBaseTest );

using namespace std;
using namespace	boost::filesystem;

muroa::CSession* CDataBaseTest::m_session = 0;
unsigned CDataBaseTest::m_testHash = 0;
int CDataBaseTest::m_testHashPos = 0;

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
	const std::string dbname("TestDB.sqlite");

	path dbpath(dbname);
	if(exists(dbpath)) {
		remove(dbpath);
	}

	m_mediaScanner = new CMediaScanner();
	m_stateDB = new muroa::CStateDB( dbname );
	m_stateDbUpdater = new CStateDbUpdater( dbname );
    m_fsScanner = new CFsScanner( m_mediaScanner );
    m_fakeCollection = new CFakeMediaCollection(".");

}

void CDataBaseTest::tearDown()
{
	delete m_fakeCollection;
	delete m_fsScanner;
	delete m_stateDbUpdater;
	delete m_stateDB;
    delete m_mediaScanner;

}

void CDataBaseTest::readMediaItem()
{
	int rc = 0;
	m_stateDB->open();

	uint32_t hash;

	prepareSession();

	m_stateDB->updateMediaColTable(m_session);
	m_stateDB->updatePlaylistRevsTable(m_session);

	CMediaItem* readItem = m_stateDB->getMediaItemByHash( m_testHash , 0);
	CPPUNIT_ASSERT_MESSAGE( "Requested Collection Item (pos 1, rev 1) does not exist.", readItem != 0 );

	CRootItem* tmpri = m_session->getMediaCol(1);
	IContentItem* cptr = tmpri->getContentPtr(CItemType(CItemType::E_MEDIAITEM), m_testHash);
	CMediaItem* writtenItem = reinterpret_cast<CMediaItem*>(cptr);
	CPPUNIT_ASSERT_MESSAGE( "Requested Collection Item does not exist in session.", writtenItem != 0 );

	cout << "Artist written/read  : " << writtenItem->getArtist() << " <-> " << readItem->getArtist() << endl;
	cout << "Album  written/read  : " << writtenItem->getAlbum() << " <-> " << readItem->getAlbum() << endl;
	cout << "Title written/read   : " << writtenItem->getTitle() << " <-> " << readItem->getTitle() << endl;
	cout << "Year  written/read   : " << writtenItem->getYear() << " <-> " << readItem->getYear() << endl;
	cout << "Duration written/read: " << writtenItem->getDuration() << " <-> " << readItem->getDuration() << endl;
	cout << "Hash written/read    : " << writtenItem->getHash() << " <-> " << readItem->getHash() << endl;
	if(writtenItem->getHash() != readItem->getHash() ) rc = 1;

	CPPUNIT_ASSERT( writtenItem->getArtist().compare(readItem->getArtist()) == 0);
	CPPUNIT_ASSERT( writtenItem->getAlbum().compare(readItem->getAlbum()) == 0);
	CPPUNIT_ASSERT( writtenItem->getTitle().compare(readItem->getTitle()) == 0);
	CPPUNIT_ASSERT( writtenItem->getYear() == readItem->getYear());
	CPPUNIT_ASSERT( writtenItem->getDuration() == readItem->getDuration());
	CPPUNIT_ASSERT( writtenItem->getHash() == readItem->getHash());
	CPPUNIT_ASSERT( writtenItem->getHash() == readItem->getHash() );

	m_stateDB->close();

	CPPUNIT_ASSERT( rc == 0 );
}


void CDataBaseTest::readGeneral() {
	m_stateDB->open();

	m_stateDB->setValue("MinMediaColRev", colMinRevVal);
	m_stateDB->setValue("MaxMediaColRev", colMaxRevVal);

	m_stateDB->setValue("MinPlaylistRev", plMinRevVal);
	m_stateDB->setValue("MaxPlaylistRev", plMaxRevVal);

	m_stateDB->setValue("MinNextlistRev", nlMinRevVal);
	m_stateDB->setValue("MaxNextlistRev", nlMaxRevVal);

	bool found;

	std::string colMinRev = m_stateDB->getValue("MinMediaColRev", found);
	CPPUNIT_ASSERT(found);
	std::string colMaxRev = m_stateDB->getValue("MaxMediaColRev", found);
	CPPUNIT_ASSERT(found);

	std::string plMinRev = m_stateDB->getValue("MinPlaylistRev", found);
	CPPUNIT_ASSERT(found);
	std::string plMaxRev = m_stateDB->getValue("MaxPlaylistRev", found);
	CPPUNIT_ASSERT(found);

	std::string nlMinRev = m_stateDB->getValue("MinNextlistRev", found);
	CPPUNIT_ASSERT(found);
	std::string nlMaxRev = m_stateDB->getValue("MaxNextlistRev", found);
	CPPUNIT_ASSERT(found);

	cout << "Collection revs: [" << colMinRev << ".." << colMaxRev << "]" << endl;
	cout << "Playlist revs:   [" << plMinRev << ".." << plMaxRev << "]" << endl;
	cout << "Nextlist revs:   [" << nlMinRev << ".." << nlMaxRev << "]" << endl;

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

	prepareSession();
	m_stateDB->updateMediaColTable(m_session);

	int rowID = m_stateDB->rowIDofColRevEntry(m_testHashPos, m_testHash, 1);

	cout << "RowID of (" << m_testHashPos << ", " << m_testHash << ", 0): "<< rowID << endl;

	m_stateDB->close();
	CPPUNIT_ASSERT( rowID != 0 );

}

void CDataBaseTest::prepareSession() {

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	vector<CMediaItem*> *mItems;

	m_fsScanner->setFileTypes(types);
	m_fsScanner->scanDir("/home/martin/Desktop", 1);
	mItems = m_fsScanner->finishScan();


	m_testHashPos = mItems->size() / 2;
	m_testHash = mItems->at(m_testHashPos)->getHash();

	m_session = new muroa::CSession("unittest");

	m_categorizer = new muroa::CMediaItemCategorizer();

	CRootItem* ri = m_categorizer->categorize(mItems);

	m_session->addMediaColRev(ri);
	m_session->addMediaColRev(ri);

	preparePlaylist();
	prepareNextlist();
}



void CDataBaseTest::preparePlaylist() {
	CRootItem* playlist = new CRootItem();

//	CCollection<CCollectionItem>* collection = m_session->getCollection(0);
//	for(int i=0; i < collection->size(); i += 2) {
//		CPlaylistItem* item = new CPlaylistItem(collection->getItem(i)->getHash());
//		playlist->insert( item, -1);
//	}
//	m_session->addPlaylistRev(playlist);
//
//	playlist = new CCollection<CPlaylistItem>();
//
//	for(int i=0; i < collection->size(); i++) {
//		CPlaylistItem* item = new CPlaylistItem(collection->getItem(i)->getHash());
//		playlist->insert( item, -1);
//	}
//	m_session->addPlaylistRev(playlist);
//
//	playlist = new CCollection<CPlaylistItem>();
//
//	for( int i=1; i < collection->size(); i += 2 ) {
//		CPlaylistItem* item = new CPlaylistItem(collection->getItem(i)->getHash());
//		playlist->insert( item, -1);
//	}
//	m_session->addPlaylistRev(playlist);
}

void CDataBaseTest::prepareNextlist() {
	CRootItem* nextlist = new CRootItem();
	// nextlist->addSomeEntries();
	// m_session->addNextlistRev(nextlist);
}


void CDataBaseTest::saveSession() {
	bool ok = true;
	prepareSession();
	m_stateDB->open();
	try {
		m_stateDB->saveSession(m_session);
	}
	catch(...) {
		ok = false;
	}
	m_stateDB->close();

	CPPUNIT_ASSERT( ok == true );
}

void CDataBaseTest::restoreSession() {
	saveSession();

	bool ok = true;
	muroa::CSession* restoredSession;
	m_stateDB->open();
	try {
		restoredSession = new muroa::CSession("unittest");
		m_stateDB->restoreSession(restoredSession);
	}
	catch(...) {
		ok = false;
	}
	if( *restoredSession != *m_session ) ok = false;

	m_stateDB->close();
	CPPUNIT_ASSERT( ok == true );
}
