/*
 * CDataBaseTest.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CDataBaseTest.h"
#include "CFakeMediaCollection.h"
#include "../CStateDB.h"
#include "../mediascanner/CStateDbUpdater.h"
#include "../mediascanner/CMediaItem.h"
#include "../mediaprocessing/CCollectionUpdater.h"
#include "../CSession.h"

#include <boost/filesystem.hpp>

#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( CDataBaseTest );

using namespace std;
using namespace	boost::filesystem;

CSession* CDataBaseTest::m_session = 0;
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

	m_stateDB = new CStateDB( dbname );
	m_stateDbUpdater = new CStateDbUpdater( dbname );
    m_colUpdater = new CCollectionUpdater( );
    m_fakeCollection = new CFakeMediaCollection(".");

}

void CDataBaseTest::tearDown()
{
	delete m_fakeCollection;
	delete m_colUpdater;
	delete m_stateDbUpdater;
	delete m_stateDB;
}

void CDataBaseTest::testDB()
{
	int rc = 0;
	m_stateDB->open();

//	vector<string> types;
//	types.push_back(".mp3");
//	types.push_back(".ogg");
//
//	CCollection<CCollectionItem>* collection = 0;
//
//	m_colUpdater->setFileTypes(types);
//	collection = m_colUpdater->walkTree("/home/martin");
//	m_testHashPos = collection->size() / 2;
//	m_testHash = collection->getItem(m_testHashPos)->getHash();
//
//	m_session = new CSession;
//	m_session->addCollectionRev(collection);
//	m_session->addCollectionRev(collection);
//
//	preparePlaylist();
//	prepareNextlist();

	prepareSession();

	m_stateDB->updateCollectionTable(m_session);
	m_stateDB->updatePlaylistRevsTable(m_session);

	m_stateDB->getCollectionItemByHash( 90039379 );

	m_stateDB->setValue("CollectionRevMin", colMinRevVal);
	m_stateDB->setValue("CollectionRevMax", colMaxRevVal);

	m_stateDB->setValue("PlaylistRevMin", plMinRevVal);
	m_stateDB->setValue("PlaylistRevMax", plMaxRevVal);

	m_stateDB->setValue("NextlistRevMin", nlMinRevVal);
	m_stateDB->setValue("NextlistRevMax", nlMaxRevVal);


	CCollectionItem* writtenItem = m_session->getCollection(1)->getItem(1);
	CCollectionItem* readItem = m_stateDB->getCollectionItemByPos(1, 1);
	cout << "Artist written/read  : " << (const char*)writtenItem->getArtist().toUtf8() << " <-> " << (const char*)readItem->getArtist().toUtf8() << endl;
	cout << "Album  written/read  : " << (const char*)writtenItem->getAlbum().toUtf8() << " <-> " << (const char*)readItem->getAlbum().toUtf8() << endl;
	cout << "Title written/read   : " << (const char*)writtenItem->getTitle().toUtf8() << " <-> " << (const char*)readItem->getTitle().toUtf8() << endl;
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



	std::string colMinRev = m_stateDB->getValue("CollectionRevMin");
	std::string colMaxRev = m_stateDB->getValue("CollectionRevMax");

	std::string plMinRev = m_stateDB->getValue("PlaylistRevMin");
	std::string plMaxRev = m_stateDB->getValue("PlaylistRevMax");

	std::string nlMinRev = m_stateDB->getValue("NextlistRevMin");
	std::string nlMaxRev = m_stateDB->getValue("NextlistRevMax");

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


	int rowID = m_stateDB->rowIDofColRevEntry(m_testHashPos, m_testHash, 0);

	cout << "RowID of (" << m_testHashPos << ", " << m_testHash << ", 0): "<< rowID << endl;

	m_stateDB->close();
	CPPUNIT_ASSERT( rowID != 0 );

}

void CDataBaseTest::prepareSession() {

	vector<string> types;
	types.push_back(".mp3");
	types.push_back(".ogg");

	CCollection<CCollectionItem>* collection = 0;

	m_colUpdater->setFileTypes(types);
	collection = m_colUpdater->walkTree("/home/martin");
	m_testHashPos = collection->size() / 2;
	m_testHash = collection->getItem(m_testHashPos)->getHash();

	m_session = new CSession;
	m_session->addCollectionRev(collection);
	m_session->addCollectionRev(collection);

	preparePlaylist();
	prepareNextlist();
}



void CDataBaseTest::preparePlaylist() {
	CCollection<CPlaylistItem>* playlist = 0;
	playlist = new CCollection<CPlaylistItem>();

	CCollection<CCollectionItem>* collection = m_session->getCollection(0);
	for(int i=0; i < collection->size(); i += 2) {
		CPlaylistItem* item = new CPlaylistItem(collection->getItem(i)->getHash());
		playlist->insert( item, -1);
	}
	m_session->addPlaylistRev(playlist);

	playlist = new CCollection<CPlaylistItem>();

	for(int i=0; i < collection->size(); i++) {
		CPlaylistItem* item = new CPlaylistItem(collection->getItem(i)->getHash());
		playlist->insert( item, -1);
	}
	m_session->addPlaylistRev(playlist);

	playlist = new CCollection<CPlaylistItem>();

	for( int i=1; i < collection->size(); i += 2 ) {
		CPlaylistItem* item = new CPlaylistItem(collection->getItem(i)->getHash());
		playlist->insert( item, -1);
	}
	m_session->addPlaylistRev(playlist);
}

void CDataBaseTest::prepareNextlist() {
	CCollection<CPlaylistItem>* nextlist = 0;
	nextlist = new CCollection<CPlaylistItem>();
	// nextlist->addSomeEntries();
	// m_session->addNextlistRev(nextlist);
}



void CDataBaseTest::StateDbUpdater() {
	vector<CMediaItem*>* col_pre;
	vector<CMediaItem*>* col_post;

	m_stateDbUpdater->open();

	col_pre = m_fakeCollection->collectionWithoutFiles(1000);

	int maxrev_pre = m_stateDbUpdater->getIntValue("CollectionRevMax");
	m_stateDbUpdater->appendCollectionRev( col_pre );

	int maxrev_post = m_stateDbUpdater->getIntValue("CollectionRevMax");
	CPPUNIT_ASSERT_MESSAGE("CStateDbUpdater::appendCollectionRev(..) did not increase CollectionRevMax by 1.", maxrev_pre + 1 == maxrev_post );

	col_post = m_stateDbUpdater->getCollectionRev( maxrev_pre );

	vector<CMediaItem*>::iterator it_pre;
	vector<CMediaItem*>::iterator it_post;

	for( it_pre = col_pre->begin(), it_post = col_post->begin(); it_pre != col_pre->end() && it_post != col_post->end() ; it_pre++, it_post++ ) {
		CMediaItem* preItem = *it_pre;
		CMediaItem* postItem = *it_post;

		CPPUNIT_ASSERT_MESSAGE("CMediaItem differ after rereading them from StateDB!", *preItem == *postItem );
	}

}


void CDataBaseTest::noDBChange() {
	vector<CMediaItem*>* col;

	m_stateDbUpdater->open();

	col = m_fakeCollection->collectionWithoutFiles(1000);

	int nrChanges;
	nrChanges = m_stateDbUpdater->appendCollectionRev( col );

	// add the same collection a second time to DB, expect nrChanges to be 0
	nrChanges = m_stateDbUpdater->appendCollectionRev( col );
	CPPUNIT_ASSERT_MESSAGE("Adding the same collection  to the database did not return zero changes! ", nrChanges == 0 );

}



void CDataBaseTest::saveSession() {
	bool ok = true;
	m_stateDB->open();
	try {
		m_stateDB->saveSession(m_session);
	}
	catch(...) {
		ok = false;
	}
	CPPUNIT_ASSERT( ok == true );
}

void CDataBaseTest::restoreSession() {

	saveSession();

	bool ok = true;
	CSession* restoredSession;
	m_stateDB->open();
	try {
		restoredSession = new CSession();
		m_stateDB->restoreSession(restoredSession);
	}
	catch(...) {
		ok = false;
	}

	int reMinColRev = restoredSession->getMinCollectionRevision();
	int minColRev = m_session->getMinCollectionRevision();
	if(reMinColRev != minColRev) {
		ok = false;
		cout << "restored min Collection Revision does not match original one." << endl;
	}

	int reMaxColRev = restoredSession->getCollectionRevision();
	int maxColRev = m_session->getCollectionRevision();
	if(reMaxColRev != maxColRev) {
		ok = false;
		cout << "restored max Collection Revision does not match original one." << endl;
	}

	int reMinPlRev = restoredSession->getMinPlaylistRevision();
	int minPlRev = m_session->getMinPlaylistRevision();
	if(reMinPlRev != minPlRev) {
		ok = false;
		cout << "restored min Playlist Revision does not match original one." << endl;
	}

	int reMaxPlRev = restoredSession->getPlaylistRevision();
	int maxPlRev = m_session->getPlaylistRevision();
	if(reMaxPlRev != maxPlRev) {
		ok = false;
		cout << "restored max Playlist Revision does not match original one." << endl;
	}

	int reMinNlRev = restoredSession->getMinNextlistRevision();
	int minNlRev = m_session->getMinNextlistRevision();
	if(reMinNlRev != minNlRev) {
		ok = false;
		cout << "restored min Nextlist Revision does not match original one." << endl;
	}

	int reMaxNlRev = restoredSession->getNextlistRevision();
	int maxNlRev = m_session->getNextlistRevision();
	if(reMaxNlRev != maxNlRev) {
		ok = false;
		cout << "restored max Nextlist Revision does not match original one." << endl;
	}

	for(int colRev = minColRev; colRev < maxColRev; colRev++ ) {
		CCollection<CCollectionItem>* col = m_session->getCollection(colRev);
		CCollection<CCollectionItem>* reCol = restoredSession->getCollection(colRev);

		CPPUNIT_ASSERT( reCol != 0 );

		if(col->size() != reCol->size() ) ok = false;

		CPPUNIT_ASSERT( reCol->size() > 0 );

		for(int i=0; i < col->size(); i++) {
			CCollectionItem* item = col->getItem(i);
			CCollectionItem* reItem = reCol->getItem(i);
			if(item->getAlbum().compare(reItem->getAlbum()) != 0 ) {
				ok = false;
				cout << "restored collection[" << i << "] Album does not match original one." << endl;
			}
			if(item->getArtist().compare(reItem->getArtist()) != 0 ) {
				ok = false;
				cout << "restored collection[" << i << "] Artist does not match original one." << endl;
			}
			if(item->getTitle().compare(reItem->getTitle()) != 0 ) {
				ok = false;
				cout << "restored collection[" << i << "] Title does not match original one." << endl;
			}
			if(item->getFilename().compare(reItem->getFilename()) != 0 ) {
				ok = false;
				cout << "restored collection[" << i << "] filename does not match original one." << endl;
			}
			if(item->getDuration() != reItem->getDuration() ) {
				ok = false;
				cout << "restored collection[" << i << "] Duration does not match original one." << endl;
			}
			if(item->getYear() != reItem->getYear() ) {
				ok = false;
				cout << "restored collection[" << i << "] Year does not match original one." << endl;
			}
		}
	}

	for(int plRev = minPlRev; plRev < maxPlRev; plRev++ ) {
		CCollection<CPlaylistItem>* pl = m_session->getPlaylist(plRev);
		CCollection<CPlaylistItem>* rePl = restoredSession->getPlaylist(plRev);

		if(pl->size() != rePl->size() ) ok = false;

		for(int i=0; i < pl->size(); i++) {
			CPlaylistItem* item = pl->getItem(i);
			CPlaylistItem* reItem = rePl->getItem(i);
			if(item->getCollectionHash() != reItem->getCollectionHash() ) {
				ok = false;
				cout << "restored playlist[" << i << "] collectionHash does not match original one." << endl;
			}
		}
	}
	CPPUNIT_ASSERT( ok == true );
}
