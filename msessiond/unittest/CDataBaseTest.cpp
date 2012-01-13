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
#include <CSession.h>
#include <CMediaItemCategorizer.h>
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

void CDataBaseTest::testDB()
{
	int rc = 0;
	m_stateDB->open();

	uint32_t hash;

	prepareSession();

	m_stateDB->updateMediaColTable(m_session);
	m_stateDB->updatePlaylistRevsTable(m_session);

	m_stateDB->getMediaItemByHash( 90039379 , 0);

	m_stateDB->setValue("CollectionRevMin", colMinRevVal);
	m_stateDB->setValue("CollectionRevMax", colMaxRevVal);

	m_stateDB->setValue("PlaylistRevMin", plMinRevVal);
	m_stateDB->setValue("PlaylistRevMax", plMaxRevVal);

	m_stateDB->setValue("NextlistRevMin", nlMinRevVal);
	m_stateDB->setValue("NextlistRevMax", nlMaxRevVal);


	CMediaItem* writtenItem = reinterpret_cast<CMediaItem*>(m_session->getMediaCol(1)->getContentPtr(CItemType(CItemType::E_MEDIAITEM), hash));
	CRootItem::iterator it = m_session->getMediaCol(1)->begin();
	CMediaItem* readItem = reinterpret_cast<CMediaItem*>(*it);
	CPPUNIT_ASSERT_MESSAGE( "Requested Collection Item (pos 1, rev 1) does not exist.", readItem != 0 );

	cout << "Artist written/read  : " << writtenItem->getArtist() << " <-> " << readItem->getArtist() << endl;
	cout << "Album  written/read  : " << writtenItem->getAlbum() << " <-> " << readItem->getAlbum() << endl;
	cout << "Title written/read   : " << writtenItem->getTitle() << " <-> " << readItem->getTitle() << endl;
	cout << "Year  written/read   : " << writtenItem->getYear() << " <-> " << readItem->getYear() << endl;
	cout << "Duration written/read: " << writtenItem->getDuration() << " <-> " << readItem->getDuration() << endl;
	cout << "Hash written/read    : " << writtenItem->getHash() << " <-> " << readItem->getHash() << endl;
	if(writtenItem->getHash() != readItem->getHash() ) rc = 1;

	m_stateDB->close();

	CPPUNIT_ASSERT( rc == 0 );
}


void CDataBaseTest::readGeneral() {
	m_stateDB->open();

	m_stateDB->setValue("CollectionRevMin", colMinRevVal);
	m_stateDB->setValue("CollectionRevMax", colMaxRevVal);

	m_stateDB->setValue("PlaylistRevMin", plMinRevVal);
	m_stateDB->setValue("PlaylistRevMax", plMaxRevVal);

	m_stateDB->setValue("NextlistRevMin", nlMinRevVal);
	m_stateDB->setValue("NextlistRevMax", nlMaxRevVal);

	bool found;

	std::string colMinRev = m_stateDB->getValue("CollectionRevMin", found);
	CPPUNIT_ASSERT(found);
	std::string colMaxRev = m_stateDB->getValue("CollectionRevMax", found);
	CPPUNIT_ASSERT(found);

	std::string plMinRev = m_stateDB->getValue("PlaylistRevMin", found);
	CPPUNIT_ASSERT(found);
	std::string plMaxRev = m_stateDB->getValue("PlaylistRevMax", found);
	CPPUNIT_ASSERT(found);

	std::string nlMinRev = m_stateDB->getValue("NextlistRevMin", found);
	CPPUNIT_ASSERT(found);
	std::string nlMaxRev = m_stateDB->getValue("NextlistRevMax", found);
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

	m_session = new muroa::CSession("name");

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

	int reMinColRev = restoredSession->getMinMediaColRev();
	int minColRev = m_session->getMinMediaColRev();
	if(reMinColRev != minColRev) {
		ok = false;
		cout << "restored min Collection Revision does not match original one." << endl;
	}

	int reMaxColRev = restoredSession->getMaxMediaColRev();
	int maxColRev = m_session->getMaxMediaColRev();
	if(reMaxColRev != maxColRev) {
		ok = false;
		cout << "restored max Collection Revision does not match original one." << endl;
	}

	int reMinPlRev = restoredSession->getMinPlaylistRev();
	int minPlRev = m_session->getMinPlaylistRev();
	if(reMinPlRev != minPlRev) {
		ok = false;
		cout << "restored min Playlist Revision does not match original one." << endl;
	}

	int reMaxPlRev = restoredSession->getMaxPlaylistRev();
	int maxPlRev = m_session->getMaxPlaylistRev();
	if(reMaxPlRev != maxPlRev) {
		ok = false;
		cout << "restored max Playlist Revision does not match original one." << endl;
	}

	int reMinNlRev = restoredSession->getMinNextlistRev();
	int minNlRev = m_session->getMinNextlistRev();
	if(reMinNlRev != minNlRev) {
		ok = false;
		cout << "restored min Nextlist Revision does not match original one." << endl;
	}

	int reMaxNlRev = restoredSession->getMaxNextlistRev();
	int maxNlRev = m_session->getMaxNextlistRev();
	if(reMaxNlRev != maxNlRev) {
		ok = false;
		cout << "restored max Nextlist Revision does not match original one." << endl;
	}

	for(int colRev = minColRev; colRev <= maxColRev; colRev++ ) {
		CRootItem* mediaCol = m_session->getMediaCol(colRev);
		CRootItem* reCol = restoredSession->getMediaCol(colRev);

		CPPUNIT_ASSERT( reCol != 0 );
		CPPUNIT_ASSERT( mediaCol != 0 );

		CPPUNIT_ASSERT(mediaCol == reCol);
	}

	for(int plRev = minPlRev; plRev < maxPlRev; plRev++ ) {
		CRootItem* pl = m_session->getPlaylist(plRev);
		CRootItem* rePl = restoredSession->getPlaylist(plRev);

		CPPUNIT_ASSERT( pl != 0 );
		CPPUNIT_ASSERT( rePl != 0 );

		CPPUNIT_ASSERT( pl == rePl );
	}
	m_stateDB->close();

	CPPUNIT_ASSERT( ok == true );
}
