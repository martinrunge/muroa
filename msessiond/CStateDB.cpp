/*
 * CStateDB.cpp
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#include "CRootItem.h"

#include <CMediaItem.h>
#include <CPlaylistItem.h>
#include <CNextlistItem.h>

#include "CStateDB.h"
#include "CSession.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace muroa {

CStateDB::CStateDB(std::string dbFileName): CStateDbBase(dbFileName),
                                            m_updateMediaItemStmt(0),
                                            m_selectMediaItemStmt(0),
                                            m_getMediaItemByPosStmt(0),
                                            m_updatePlaylistItemStmt(0),
                                            m_selectPlaylistItemStmt(0),
                                            m_selectPlaylistRevStmt(0),
                                            m_updateNextlistItemStmt(0),
                                            m_selectNextlistItemStmt(0),
                                            m_selectNextlistRevStmt(0)
{
}

CStateDB::~CStateDB() {
}

int CStateDB::open() {
	int rc = CStateDbBase::open();
	if( rc == 0 ){

		prepareUpdateMediaItemStmt();

		createPlaylistRevisionsTable();
		createNextlistRevisionsTable();

		prepareSelectMediaItemStmt();
		prepareGetMediaItemByPosStmt();

		prepareSelectPlaylistRevStmt();
		prepareUpdatePlaylistItemStmt();
		prepareSelectPlaylistItemStmt();

		prepareSelectNextlistRevStmt();
		prepareUpdateNextlistItemStmt();
		prepareSelectNextlistItemStmt();
	}
	return rc;
}

int CStateDB::close() {
	finalizeUpdateNextlistItemStmt();
	finalizeSelectNextlistItemStmt();
	finalizeSelectNextlistRevStmt();

	finalizeUpdatePlaylistItemStmt();
	finalizeSelectPlaylistItemStmt();
	finalizeSelectPlaylistRevStmt();

	finalizeSelectMediaItemStmt();
	finalizeGetMediaItemByPosStmt();
	finalizeUpdateMediaItemStmt();

	int rc = CStateDbBase::close();
	return rc;
}

void CStateDB::saveSession(CSession const * const session) {
	int colMinRevVal = session->getMinMediaColRev();
	int colMaxRevVal = session->getMaxMediaColRev();
	setValue("MinMediaColRev", colMinRevVal);
	setValue("MaxMediaColRev", colMaxRevVal);
	updateMediaColTable(session);

	int minPlRevVal = session->getMinPlaylistRev();
	int maxPlRevVal = session->getMaxPlaylistRev();
	setValue("MinPlaylistRev", minPlRevVal);
	setValue("MaxPlaylistRev", maxPlRevVal);
	updatePlaylistRevsTable(session);

	int minNlRevVal = session->getMinNextlistRev();
	int maxNlRevVal = session->getMaxNextlistRev();
	setValue("MinNextlistRev", minNlRevVal);
	setValue("MaxNextlistRev", maxNlRevVal);
    updateNextlistRevsTable(session);
}

void CStateDB::restoreSession(CSession * const session) {
	restoreMediaCols(session);
	restorePlaylists(session);
	// restoreNextlists(session);
	// repairSession(session);
}

void CStateDB::updatePlaylistRevsTable(CSession const * const session, int minrev, int maxrev ) {
	if(minrev == -1) minrev = session->getMinPlaylistRev();
	if(maxrev == -1) maxrev = session->getMaxPlaylistRev();

	bool found = false;
	uint32_t maxPlRev = getIntValue("MaxPlaylistRev", found);
	assert(found);


	for(int rev = minrev; rev <= maxrev; rev++) {
		if(rev != 0) { // rev 0 is an empty dummy revision already created by c-tor.
			CRootItem* playlist = session->getPlaylist(rev);

			CRootItem::iterator it(playlist->begin());
			for(int i=0; it != playlist->end(); it++, i++ ) {
				CItemBase* item_b = *it;
				if(item_b->type() == CItemType::E_PLAYLISTITEM) {
					CPlaylistItem* item = reinterpret_cast<CPlaylistItem*>(item_b);
					updatePlaylistItem(i, item, rev, 0);
				}
			}
		}
	}

	if(maxrev > maxPlRev) {
		setValue("MaxPlaylistRev", maxrev);
	}

}

void CStateDB::updateNextlistRevsTable(CSession const * const session, int minrev, int maxrev ) {
	if(minrev == -1) minrev = session->getMinNextlistRev();
	if(maxrev == -1) maxrev = session->getMaxNextlistRev();

	bool found = false;
	uint32_t maxNlRev = getIntValue("MaxNextlistRev", found);
	assert(found);


	for(int rev = minrev; rev <= maxrev; rev++) {
		if(rev != 0) { // rev 0 is an empty dummy revision already created by c-tor.
			CRootItem* nextlist = session->getNextlist(rev);

			CRootItem::iterator it(nextlist->begin());
			for(int i=0; it != nextlist->end(); it++, i++ ) {
				CItemBase* item_b = *it;
				if(item_b->type() == CItemType::E_NEXTLISTITEM) {
					CNextlistItem* item = reinterpret_cast<CNextlistItem*>(item_b);
					updateNextlistItem(i, item, rev, 0, 0);
				}
			}
		}
	}

	if(maxrev > maxNlRev) {
		setValue("MaxNextlistRev", maxrev);
	}
}



void CStateDB::restoreMediaCols(CSession * const session) {
	if(m_db == 0) throw(CApiMisuseException("Calling restoreCollection(), but DB not opened."));
	bool found;

	int minRev =  getIntValue("MinMediaColRev", found);
	if(!found) return;
	int maxRev = getIntValue("MaxMediaColRev", found);
	if(!found) return;

	session->setMinMediaColRev( minRev );
	session->setMaxMediaColRev( minRev );  // max rev will be reached by calling addMediaVolRev() several times.

	for(int rev = minRev; rev <= maxRev; rev++) {
		if(rev != 0) {   // rev 0 is an empty dummy revision already created by c-tor.
			CRootItem* ri = getMediaColRev(rev);
			session->addMediaColRev(ri);
		}
	}
}

CRootItem* CStateDB::getMediaColRev(int rev) {
	CRootItem* mediaCol = new CRootItem();

	CMediaItem* item = (CMediaItem*)0xffFFffFF;
	int pos = 0;
	do {
		item = getMediaItemByPos(pos, rev, mediaCol );
		pos++;
		if(item) {

			ostringstream oss;
			oss << "/" << item->getArtist() << "/" << item->getAlbum();

			cerr << oss.str();

			CCategoryItem* parent = mediaCol->getCategoryPtr(oss.str());
			if(parent == 0) {
				parent = mediaCol->mkPath(oss.str());
			}
			mediaCol->addContentItem(item, parent);
		}
	} while(item != 0);

	return mediaCol;
}


void CStateDB::updateMediaColTable( CSession const * const session, int minrev, int maxrev ) {

	beginTansaction();

	if(minrev == -1) minrev = session->getMinMediaColRev();
	if(maxrev == -1) maxrev = session->getMaxMediaColRev();

	for(int rev = minrev; rev <= maxrev; rev++) {
		if (rev != 0) {  // rev 0 is an empty dummy revision already created by c-tor.
			CRootItem* mediaCol = session->getMediaCol(rev);

			CRootItem::iterator it(mediaCol->begin());

			for(int pos = 0; it != mediaCol->end(); it++, pos++) {
				CItemBase* item_b = *it;
				if(item_b->type() == CItemType::E_MEDIAITEM) {
					CMediaItem* item = reinterpret_cast<CMediaItem*>(item_b);
					updateMediaItem( item );

					updateCollectionRevItem(pos, item->getHash(), rev );
				}
			}
		}
	}
	endTransaction();
}

void CStateDB::restorePlaylists(CSession * const session) {
	if(m_db == 0) throw(CApiMisuseException("Calling restorePlaylists(), but DB not opened."));
	bool found;

	int minRev = getIntValue("MinPlaylistRev", found);
	if(!found) return;
	int maxRev = getIntValue("MaxPlaylistRev", found);
	if(!found) return;

	session->setMinPlaylistRev( minRev );
	session->setMaxPlaylistRev( minRev );  // max rev will be set by addPlaylistRev();

	for(int rev = minRev; rev <= maxRev; rev++) {
		if(rev != 0) { // rev 0 is an empty dummy revision already created by c-tor.
			CRootItem* playlist = new CRootItem();

			CPlaylistItem* item;
			int pos = 0;
			do {
				item = getPlaylistItemByPos(pos, rev, playlist);
				pos++;
				if(item) {
					CCategoryItem* parent = playlist->getCategoryPtr();
					playlist->addContentItem(item, parent);
				}
			} while(item != 0);
			session->addPlaylistRev(playlist);
		}
	}
}

void CStateDB::restoreNextlists(CSession * const session) {
	if(m_db == 0) throw(CApiMisuseException("Calling restoreNextlists(), but DB not opened."));
	bool found;

	int minRev = getIntValue("MinNextlistRev", found);
	if(!found) return;
	int maxRev = getIntValue("MaxNextlistRev", found);
	if(!found) return;

	session->setMinNextlistRev( minRev );
	session->setMaxNextlistRev( minRev );  // max rev will be set by addNextlistRev();

	for(int rev = minRev; rev < maxRev; rev++) {
		if(rev != 0) {  // rev 0 is an empty dummy revision already created by c-tor.
			CRootItem* nextlist = new CRootItem();

			CNextlistItem* item;
			int pos = 0;
			do {
				item = getNextlistItemByPos(pos, rev, nextlist);
				pos++;
				if(item) {
					CCategoryItem* parent = nextlist->getCategoryPtr();
					nextlist->addContentItem(item, parent);
				}
			} while(item != 0);
			session->addNextlistRev(nextlist);
		}
	}
}

/**
 *   check for playlist entries not in collection. If any, remove them and create new playlist revision.
 */
void CStateDB::repairSession(CSession* session) {
	CRootItem* mediaCol = session->getMediaCol();
	CRootItem* playlist = session->getPlaylist();
	CRootItem* nextlist = session->getNextlist();

	CRootItem* newPlaylist = new CRootItem();
	bool unknownItemInPlaylist = false;

	CRootItem::iterator it(playlist->begin());
	for(; it != playlist->end(); it++) {
		CCategoryItem* base = newPlaylist->getBase();
		CPlaylistItem* plItem = new CPlaylistItem(newPlaylist, base );

		unsigned hash = plItem->getMediaItemHash();
		CItemBase* item_b = mediaCol->getContentPtr(CItemType::E_MEDIAITEM, hash);
		CMediaItem* item = reinterpret_cast<CMediaItem*>(item_b);
		if( item == 0 ) {
			unknownItemInPlaylist = true;
		}
		else {
			newPlaylist->addContentItem(plItem, base);
		}
	}

	if( unknownItemInPlaylist ) {
		session->addPlaylistRev( newPlaylist );
		updatePlaylistRevsTable(session, session->getMaxPlaylistRev(), session->getMaxPlaylistRev());
	}
	else {
		delete newPlaylist;
	}

}

void CStateDB::updateMediaItem( CMediaItem* item ) {
	assert(m_updateMediaItemStmt != 0);

	int retval = sqlite3_bind_int(m_updateMediaItemStmt, 1, item->getHash());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'hash' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 2, item->getFilename().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'filename' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 3, item->getArtist().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'Artist' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 4, item->getAlbum().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'album' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 5, item->getTitle().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'title' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 6, item->getYear());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'year' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 7, item->getDuration());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'duration' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 8, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_played' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 9, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_skipped' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 10, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_repeated' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 11, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'rating' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_step( m_updateMediaItemStmt );

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping m_updateColItemStmt: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_reset(m_updateMediaItemStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting m_updateColItemStmt statement: " << sqlite3_errmsg(m_db);
	}
}


CMediaItem* CStateDB::getMediaItemByPos(int colPos, int colRev, CRootItem* ri) {
	CMediaItem* item = 0;

	assert(m_getMediaItemByPosStmt != 0);

	int retval = sqlite3_bind_int(m_getMediaItemByPosStmt, 1, colPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colPos' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_getMediaItemByPosStmt, 2, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_getMediaItemByPosStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getMediaItemFromStmt(m_getMediaItemByPosStmt, ri);
			num_found++;
 			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db) << endl;
			break;
		}
	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping getColItemByPos: " << sqlite3_errmsg(m_db);
	}
	retval = sqlite3_reset(m_getMediaItemByPosStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting getColItemByPos statement: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CMediaItem* CStateDB::getMediaItemByHash(unsigned hash, CRootItem* ri) {
	CMediaItem* item;

	int retval = sqlite3_bind_int(m_selectMediaItemStmt, 1, hash);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'hash' to selectColItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	do {
		retval = sqlite3_step( m_selectMediaItemStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getMediaItemFromStmt(m_selectMediaItemStmt, ri);
			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db);
			break;
		}

	}while (retval == SQLITE_ROW);
	if(retval != SQLITE_DONE) {
		cerr << "Error stepping selectColItemStmt: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_reset(m_selectMediaItemStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting selectColItemStmt: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CMediaItem* CStateDB::getMediaItemFromStmt(sqlite3_stmt *pStmt, CRootItem* ri) {
	CMediaItem* item = new CMediaItem(ri);
	// (hash INTEGER, file TEXT, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	int numCol = sqlite3_column_count(pStmt);
	// Scerr << "result has " << numCol << " columns" << endl;

	int hash = sqlite3_column_int(pStmt, 0);

	const unsigned char *filename = sqlite3_column_text(pStmt, 1);
	int filenameSize = sqlite3_column_bytes(pStmt, 1);
	item->setFilename( (const char*)filename );

	const unsigned char *artist = sqlite3_column_text(pStmt, 2);
	int artistSize = sqlite3_column_bytes(pStmt, 2);
	item->setArtist( (const char*)artist );

	const unsigned char *album = sqlite3_column_text(pStmt, 3);
	int albumSize = sqlite3_column_bytes(pStmt, 3);
	item->setAlbum( (const char*)album );

	const unsigned char *title = sqlite3_column_text(pStmt, 4);
	int titleSize = sqlite3_column_bytes(pStmt, 4);
	item->setTitle( (const char*)title );

	int year = sqlite3_column_int(pStmt, 5);
	item->setYear( year );

	int duration = sqlite3_column_int(pStmt, 6);
	item->setDuration( duration );

//	cerr << "SELECT: " << filename << " " << artist << " " << album << " " << title << endl;
	return item;
}


void CStateDB::updatePlaylistItem( int plPos, CPlaylistItem* item, int plRev, int colRev ) {
	int rowid = rowIDofPlRevEntry(plPos, item->getMediaItemHash(), plRev, colRev);
	if(rowid == 0) {  // this entry does not exist yet
		int retval = sqlite3_bind_int(m_updatePlaylistItemStmt, 1, plPos);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'plPos' to updatePlaylistItem statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updatePlaylistItemStmt, 2, item->getMediaItemHash());
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'hash' to updatePlaylistItem statement: " << sqlite3_errmsg(m_db) << endl;
		}
		retval = sqlite3_bind_int(m_updatePlaylistItemStmt, 3, plRev);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'plRev' to updatePlaylistItem statement: " << sqlite3_errmsg(m_db) << endl;
		}
		retval = sqlite3_bind_int(m_updatePlaylistItemStmt, 4, colRev);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'colRev' to updatePlaylistItem statement: " << sqlite3_errmsg(m_db) << endl;
		}

		do {
			retval = sqlite3_step( m_updatePlaylistItemStmt );
		}while (retval == SQLITE_ROW);

		if(retval != SQLITE_DONE) {
			cerr << "Error stepping updatePlaylistItemStmt: " << sqlite3_errmsg(m_db);
		}
		retval = sqlite3_reset( m_updatePlaylistItemStmt );
		if(retval != SQLITE_OK) {
			cerr << "Error resetting m_updatePlaylistItemStmt statement: " << sqlite3_errmsg(m_db);
		}
	}
}

void CStateDB::updateNextlistItem( int nlPos, CNextlistItem* item, int nlRev, int plRev, int colRev ) {
	int rowid = rowIDofNlRevEntry(nlPos, item->getMediaItemHash(), item->getPlaylistItemHash(), nlRev, plRev, colRev);
	if(rowid == 0) {  // this entry does not exist yet
		int retval = sqlite3_bind_int(m_updateNextlistItemStmt, 1, nlPos);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'plPos' to m_updateNextlistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updateNextlistItemStmt, 2, item->getMediaItemHash());
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'mediaItemHash' to m_updateNextlistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updateNextlistItemStmt, 3, item->getPlaylistItemHash());
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'playlistItemHash' to m_updateNextlistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updateNextlistItemStmt, 4, nlRev);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'nlRev' to m_updateNextlistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updateNextlistItemStmt, 5, plRev);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'plRev' to m_updateNextlistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updateNextlistItemStmt, 6, colRev);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'colRev' to m_updateNextlistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
		}

		do {
			retval = sqlite3_step( m_updateNextlistItemStmt );
		}while (retval == SQLITE_ROW);

		if(retval != SQLITE_DONE) {
			cerr << "Error stepping m_updateNextlistItemStmt: " << sqlite3_errmsg(m_db);
		}
		retval = sqlite3_reset( m_updateNextlistItemStmt );
		if(retval != SQLITE_OK) {
			cerr << "Error resetting m_updatePlaylistItemStmt statement: " << sqlite3_errmsg(m_db);
		}
	}
}

int CStateDB::rowIDofPlRevEntry(int plPos, int colHash, int plRev, int colRev) {
	int retval = sqlite3_bind_int(m_selectPlaylistRevStmt, 1, plPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'plPos' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectPlaylistRevStmt, 2, colHash);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colHash' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectPlaylistRevStmt, 3, plRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'plRev' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectPlaylistRevStmt, 4, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_selectPlaylistRevStmt );
		switch(retval) {
		case SQLITE_ROW:
			rowid =	sqlite3_column_int(m_selectPlaylistRevStmt, 0);
			num_found++;
 			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db) << endl;
			break;
		}

	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping table playlistRevs: " << sqlite3_errmsg(m_db);
	}
	retval = sqlite3_reset(m_selectPlaylistRevStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting statement: " << sqlite3_errmsg(m_db);
	}
	return rowid;
}

int CStateDB::rowIDofNlRevEntry(int nlPos, int colHash, int plHash, int nlRev, int plRev, int ColRev) {

}

CPlaylistItem* CStateDB::getPlaylistItemFromStmt(sqlite3_stmt *pStmt, CRootItem* ri) {
	unsigned hash = sqlite3_column_int(pStmt, 0);
	unsigned plID = sqlite3_column_int(pStmt, 1);
	CPlaylistItem* item = new CPlaylistItem( hash, plID );
	return item;
}

CPlaylistItem* CStateDB::getPlaylistItemByPos(int pos, int rev, CRootItem* ri) {
	CPlaylistItem* item = 0;

	int retval = sqlite3_bind_int(m_selectPlaylistItemStmt, 1, pos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'pos' to selectPlaylistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_bind_int(m_selectPlaylistItemStmt, 2, rev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'rev' to selectPlaylistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_bind_int(m_selectPlaylistItemStmt, 3, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to selectPlaylistItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	do {
		retval = sqlite3_step( m_selectPlaylistItemStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getPlaylistItemFromStmt(m_selectPlaylistItemStmt, ri);
			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db);
			break;
		}

	}while (retval == SQLITE_ROW);
	if(retval != SQLITE_DONE) {
		cerr << "Error stepping selectPlaylistItemStmt: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_reset(m_selectPlaylistItemStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting selectPlaylistItemStmt: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CNextlistItem* CStateDB::getNextlistItemByPos(int pos, int rev, CRootItem* ri) {

}


void CStateDB::createPlaylistRevisionsTable() {
	createTable("playlistRevs","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, plPos INTEGER, colHash INTEGER, plID INTEGER, plRev INTEGER, ColRev INTEGER, FOREIGN KEY(colHash) REFERENCES collection(hash))");
}

void CStateDB::createNextlistRevisionsTable() {
	createTable("nextlistRevs","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, nlPos INTEGER, colHash INTEGER, plID INTEGER, nlRev INTEGER, plRev INTEGER, ColRev INTEGER)");
}

void CStateDB::createRevisionMatchTable() {
	createTable("revMatch","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, colRev INTEGER, plRev INTEGER, nlRev INTEGER)");
}


void CStateDB::prepareUpdatePlaylistItemStmt() {
	prepareStmt(&m_updatePlaylistItemStmt, "INSERT OR REPLACE INTO playlistRevs " \
			"( plPos, colHash, plID, plRev, colRev) " \
			" VALUES " \
		    "(?,?,?,?,?)");
}

void CStateDB::prepareUpdateMediaItemStmt() {
	prepareStmt(&m_updateMediaItemStmt, "INSERT OR REPLACE INTO collection " \
			"( hash, file, artist, album, title, year, duration, num_played, num_skipped, num_repeated, rating) " \
			" VALUES " \
		    "(?,?,?,?,?,?,?,?,?,?,?)");
}

void CStateDB::finalizeUpdateMediaItemStmt() {
	finalizeStmt( &m_updateMediaItemStmt );
}


void CStateDB::prepareSelectMediaItemStmt() {
	prepareStmt(&m_selectMediaItemStmt, "SELECT * FROM collection WHERE hash=?");
}

void CStateDB::finalizeSelectMediaItemStmt() {
	finalizeStmt(&m_selectMediaItemStmt);
}

void CStateDB::prepareGetMediaItemByPosStmt() {
	prepareStmt(&m_getMediaItemByPosStmt, "SELECT * FROM collection INNER JOIN collectionRevs ON collection.hash=collectionRevs.colHash WHERE collectionRevs.colPos=? AND collectionRevs.colRev=?");
}

void CStateDB::finalizeGetMediaItemByPosStmt() {
	finalizeStmt( &m_getMediaItemByPosStmt );
}

void CStateDB::finalizeUpdatePlaylistItemStmt() {
	finalizeStmt( &m_updatePlaylistItemStmt );
}

void CStateDB::prepareSelectPlaylistRevStmt() {
	prepareStmt(&m_selectPlaylistRevStmt, "SELECT rowid FROM playlistRevs WHERE plPos=? AND colHash=? AND plID=? AND plRev=? AND colRev=?");
}

void CStateDB::finalizeSelectPlaylistRevStmt() {
	finalizeStmt( &m_selectPlaylistRevStmt );
}

void CStateDB::prepareSelectPlaylistItemStmt() {
	prepareStmt(&m_selectPlaylistItemStmt, "SELECT colHash, plID FROM playlistRevs WHERE plPos=? AND plRev=? AND colRev=?");
}

void CStateDB::finalizeSelectPlaylistItemStmt() {
	finalizeStmt( &m_selectPlaylistItemStmt );
}

void CStateDB::prepareUpdateNextlistItemStmt() {

}

void CStateDB::finalizeUpdateNextlistItemStmt() {
	finalizeStmt( &m_updateNextlistItemStmt );
}

void CStateDB::prepareSelectNextlistItemStmt() {

}

void CStateDB::finalizeSelectNextlistItemStmt() {
	finalizeStmt( &m_selectNextlistItemStmt );
}

void CStateDB::prepareSelectNextlistRevStmt() {

}

void CStateDB::finalizeSelectNextlistRevStmt() {
	finalizeStmt( &m_selectNextlistRevStmt );
}

void CStateDB::prepareUpdateRevMatchStmt() {
	prepareStmt(&m_updateRevMatchStmt, "INSERT OR REPLACE INTO revMatch " \
			"(colRev, plRev, nlRev) " \
			"VALUES "\
			"(?,?,?)");
}

void CStateDB::finalizeUpdateRevMatchStmt() {
	finalizeStmt( &m_updateRevMatchStmt );
}

void CStateDB::prepareColRevForPlRevStmt() {
	prepareStmt(&m_colRevForPlRevStmt, "SELECT colRev FROM revMatch WHERE plRev=?");
}

void CStateDB::finmalizeColRevForPlRevStmt() {
	finalizeStmt(&m_colRevForPlRevStmt);
}


unsigned CStateDB::getColRevForPlRev(unsigned plRev) {
	int retval;
	bool found = false;
	uint32_t hash;
	unsigned colRev = 0;
	retval = sqlite3_bind_int(m_colRevForPlRevStmt, 1, plRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'plRev' to m_colRevForPlRevStmt: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_colRevForPlRevStmt );
		switch(retval) {
		case SQLITE_ROW:
			colRev = sqlite3_column_int(m_colRevForPlRevStmt, 1);
			num_found++;
 			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db) << endl;
			break;
		}
	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping : getColRevForPlRev" << sqlite3_errmsg(m_db);
		found = false;
		hash = 0;
	}

	retval = sqlite3_reset(m_colRevForPlRevStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting m_colRevForPlRevStmt: " << sqlite3_errmsg(m_db);
	}
	if (num_found > 0) {
		found = true;
		if(num_found > 1) {
			cerr << "CStateDbBase::getColRevForPlRev ( plRev = " << plRev
				 << ") found more than once (" << num_found
				 << ") in state DB." << endl;
		}
	}
	return colRev;
}

unsigned CStateDB::getColRevForNlRev(unsigned nlRev) {

}


}
