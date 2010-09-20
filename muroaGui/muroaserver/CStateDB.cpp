/*
 * CStateDB.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CStateDB.h"

#include "CSession.h"

#include <iostream>
#include <sstream>

#include <QDebug>

using namespace std;

CStateDB::CStateDB(std::string dbFileName) : m_dbFileName( dbFileName ),
		                                     m_db(0),
		                                     m_updateColItemStmt(0),
		                                     m_updateColRevStmt(0),
		                                     m_selectColItemStmt(0),
		                                     m_selectColRevStmt(0),
		                                     m_getColItemByPosStmt(0),
		                                     m_updatePlaylistItemStmt(0),
		                                     m_selectPlaylistItemStmt(0),
		                                     m_selectPlRevStmt(0)
{
}

CStateDB::~CStateDB() {
	close();
}


int CStateDB::open() {
	int rc = sqlite3_open(m_dbFileName.c_str(), &m_db);
	if( rc ){
	    cerr << "Can't open database: " << sqlite3_errmsg(m_db) << endl;
	    sqlite3_close(m_db);
	    m_db = 0;
    }
	else {
		prepareStmt(&m_beginTransactionStmt, "BEGIN TRANSACTION");
		prepareStmt(&m_endTransactionStmt, "COMMIT");

		createGeneralTable();
		createCollectionTable();
		createCollectionRevisionsTable();

		createPlaylistRevisionsTable();
		createNextlistRevisionsTable();

		prepareUpdateColItemStmt();
		prepareUpdateColRevStmt();
		prepareSelectColItemStmt();
		prepareSelectColRevStmt();
		prepareGetColItemByPosStmt();

		prepareSelectPlRevStmt();
		prepareUpdatePlaylistItemStmt();
		prepareSelectPlaylistItemStmt();
	}
}

int CStateDB::close() {
	finalizeUpdatePlaylistItemStmt();
	finalizeSelectPlaylistItemStmt();
	finalizeSelectPlRevStmt();

	finalizeUpdateColItemStmt();
	finalizeUpdateColRevStmt();
	finalizeSelectColItemStmt();
    finalizeSelectColRevStmt();
	finalizeGetColItemByPosStmt();

	finalizeStmt(&m_beginTransactionStmt);
	finalizeStmt(&m_endTransactionStmt);

	if(m_db != 0) {
		sqlite3_close(m_db);
	}
}

std::string CStateDB::getValue(std::string key) {
	sqlite3_stmt *pStmt;
	const char *pzTail;
	string value;

	stringstream ss;
	ss << "SELECT value FROM general WHERE key='" << key << "'";
	string sql_stmt = ss.str();

	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	do {
		CCollectionItem* item;

		retval = sqlite3_step( pStmt );
		switch(retval) {
		case SQLITE_ROW:
		{
			const unsigned char *val = sqlite3_column_text(pStmt, 0);
			int valSize = sqlite3_column_bytes(pStmt, 0);
			value = reinterpret_cast<const char*>(val);
			break;
		}
		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db);
			break;
		}

	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
	return value;
}

void CStateDB::setValue(std::string key, std::string value) {
	sqlite3_stmt *pStmt;
	const char *pzTail;

	stringstream ss;
	ss << "INSERT OR REPLACE INTO general (key, value) VALUES "
	   << "('" << key << "','" << value << "')";
	string sql_stmt = ss.str();

	// cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_step( pStmt );
	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}

void CStateDB::setValue(std::string key, int value) {
	stringstream ss;
	ss << value;
	setValue(key, ss.str());
}


void CStateDB::saveSession(CSession const * const session) {
	int colMinRevVal = session->getMinCollectionRevision();
	int colMaxRevVal = session->getCollectionRevision();
	setValue("CollectionRevMin", colMinRevVal);
	setValue("CollectionRevMax", colMaxRevVal);
	updateCollectionTable(session);

	int plMinRevVal = session->getMinPlaylistRevision();
	int plMaxRevVal = session->getPlaylistRevision();
	setValue("PlaylistRevMin", plMinRevVal);
	setValue("PlaylistRevMax", plMaxRevVal);
	updatePlaylistRevsTable(session);

	int nlMinRevVal = session->getMinNextlistRevision();
	int nlMaxRevVal = session->getNextlistRevision();
	setValue("NextlistRevMin", nlMinRevVal);
	setValue("NextlistRevMax", nlMaxRevVal);
    updateNextlistRevsTable(session);
}

void CStateDB::restoreSession(CSession * const session) {
	restoreCollection(session);
	restorePlaylists(session);
	//restoreNextlists(session);
}

void CStateDB::createGeneralTable() {
	createTable("general", "(key TEXT UNIQUE, value TEXT)");
}

void CStateDB::createCollectionTable( ) {
	createTable("collection" , "(hash INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, title TEXT, year INTEGER, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)");
}

void CStateDB::createCollectionRevisionsTable() {
	createTable("collectionRevs" , "(colPos INTEGER, colHash INTEGER REFERENCES collection (hash),  ColRev INTEGER)");
}

void CStateDB::createPlaylistRevisionsTable() {
	createTable("playlistRevs","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, plPos INTEGER, colHash INTEGER, plRev INTEGER, ColRev INTEGER, FOREIGN KEY(colHash) REFERENCES collection(hash))");
}

void CStateDB::createNextlistRevisionsTable() {
	createTable("nextlistRevs","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, nlPos INTEGER, plPos INTEGER, nlRev INTEGER, plRev INTEGER)");
}

void CStateDB::updateCollectionTable( CSession const * const session, int minrev, int maxrev ) {

	beginTansaction();

	if(minrev == -1) minrev = session->getMinCollectionRevision();
	if(maxrev == -1) maxrev = session->getCollectionRevision();

	for(int rev = minrev; rev < maxrev; rev++) {
		CCollection<CCollectionItem>* collection = session->getCollection(rev);

		for(int i = 0; i < collection->size(); i++) {
			CCollectionItem* item = collection->getItem(i);
			updateCollectionItem(item);
			updateCollectionRevItem(i, item->getHash(), rev );
		}
	}

	endTransaction();
}

void CStateDB::updateCollectionItem( CCollectionItem* item ) {
	assert(m_updateColItemStmt != 0);

	int retval = sqlite3_bind_int(m_updateColItemStmt, 1, item->getHash());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'hash' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateColItemStmt, 2, item->getFilename().toUtf8().constData(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'filename' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateColItemStmt, 3, item->getArtist().toUtf8().constData(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'Artist' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateColItemStmt, 4, item->getAlbum().toUtf8().constData(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'album' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateColItemStmt, 5, item->getTitle().toUtf8().constData(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'title' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateColItemStmt, 6, item->getYear());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'year' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateColItemStmt, 7, item->getDuration());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'duration' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateColItemStmt, 8, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_played' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateColItemStmt, 9, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_skipped' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateColItemStmt, 10, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_repeated' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateColItemStmt, 11, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'rating' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_step( m_updateColItemStmt );

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping m_updateColItemStmt: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_reset(m_updateColItemStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting m_updateColItemStmt statement: " << sqlite3_errmsg(m_db);
	}
}

void CStateDB::updateCollectionRevItem( int pos, int hash, int rev ) {

	int rowid = rowIDofColRevEntry(pos, hash, rev);
	if(rowid == 0) {  // this entry does not exist yet
		int retval = sqlite3_bind_int(m_updateColRevStmt, 1, pos);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'pos' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updateColRevStmt, 2, hash);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'hash' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
		}
		retval = sqlite3_bind_int(m_updateColRevStmt, 3, rev);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'rev' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
		}

		do {
			retval = sqlite3_step( m_updateColRevStmt );
		}while (retval == SQLITE_ROW);

		if(retval != SQLITE_DONE) {
			cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
		}
		retval = sqlite3_reset(m_updateColRevStmt);
		if(retval != SQLITE_OK) {
			cerr << "Error resetting m_updateColRevStmt statement: " << sqlite3_errmsg(m_db);
		}
	}
}

void CStateDB::updatePlaylistItem( int plPos, CPlaylistItem* item, int plRev, int colRev ) {
	int rowid = rowIDofPlRevEntry(plPos, item->getCollectionHash(), plRev, colRev);
	if(rowid == 0) {  // this entry does not exist yet
		int retval = sqlite3_bind_int(m_updatePlaylistItemStmt, 1, plPos);
		if(retval != SQLITE_OK) {
			cerr << "Error binding value 'plPos' to updatePlaylistItem statement: " << sqlite3_errmsg(m_db) << endl;
		}

		retval = sqlite3_bind_int(m_updatePlaylistItemStmt, 2, item->getCollectionHash());
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

void CStateDB::updateNextlistItem( int nlPos, CPlaylistItem* item, int nlRev, int plRev ) {

}

CCollectionItem* CStateDB::getCollectionItemByHash(unsigned hash) {
	CCollectionItem* item;

	int retval = sqlite3_bind_int(m_selectColItemStmt, 1, hash);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'hash' to selectColItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	do {
		retval = sqlite3_step( m_selectColItemStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getCollectionItemFromStmt(m_selectColItemStmt);
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

	retval = sqlite3_reset(m_selectColItemStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting selectColItemStmt: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CCollectionItem* CStateDB::getCollectionItemByPos(int colPos, int colRev) {
	CCollectionItem* item = 0;

	assert(m_getColItemByPosStmt != 0);

	int retval = sqlite3_bind_int(m_getColItemByPosStmt, 1, colPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colPos' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_getColItemByPosStmt, 2, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_getColItemByPosStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getCollectionItemFromStmt(m_getColItemByPosStmt);
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
	retval = sqlite3_reset(m_getColItemByPosStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting getColItemByPos statement: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CCollectionItem* CStateDB::getCollectionItemFromStmt(sqlite3_stmt *pStmt) {
	CCollectionItem* item = new CCollectionItem;
	// (hash INTEGER, file TEXT, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	int numCol = sqlite3_column_count(pStmt);
	// Scerr << "result has " << numCol << " columns" << endl;

	int hash = sqlite3_column_int(pStmt, 0);

	const unsigned char *filename = sqlite3_column_text(pStmt, 1);
	int filenameSize = sqlite3_column_bytes(pStmt, 1);
	item->setFilename( QString::fromUtf8((const char*) filename) );

	const unsigned char *artist = sqlite3_column_text(pStmt, 2);
	int artistSize = sqlite3_column_bytes(pStmt, 2);
	item->setArtist( QString::fromUtf8((const char*)artist ));

	const unsigned char *album = sqlite3_column_text(pStmt, 3);
	int albumSize = sqlite3_column_bytes(pStmt, 3);
	item->setAlbum( QString::fromUtf8((const char*) album ));

	const unsigned char *title = sqlite3_column_text(pStmt, 4);
	int titleSize = sqlite3_column_bytes(pStmt, 4);
	item->setTitle( QString::fromUtf8((const char*) title) );

	int year = sqlite3_column_int(pStmt, 5);
	item->setYear( year );

	int duration = sqlite3_column_int(pStmt, 6);
	item->setDuration( duration );

//	cerr << "SELECT: " << filename << " " << artist << " " << album << " " << title << endl;
	return item;
}

CPlaylistItem* CStateDB::getPlaylistItemFromStmt(sqlite3_stmt *pStmt) {
	unsigned hash = sqlite3_column_int(pStmt, 0);
	CPlaylistItem* item = new CPlaylistItem( hash );
	return item;
}

CPlaylistItem* CStateDB::getPlaylistItemByPos(int pos, int rev) {
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
			item = getPlaylistItemFromStmt(m_selectPlaylistItemStmt);
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

CPlaylistItem* CStateDB::getNextlistItemByPos(int pos, int rev) {

}

/** \brief  Create a database table if it does not yet exist
 *
 *
 * \param   name   Name of the table to create
 * \param   schema Table schema. e.g. "(rev_id INTEGER, rev INTEGER)"
 * \exception  todexception on failure
 */
void CStateDB::createTable(std::string name, std::string schema) {
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */

	stringstream ss;
	ss << "CREATE TABLE IF NOT EXISTS " << name << " " << schema;
	string sql_stmt = ss.str();

	// cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_step( pStmt );

	if(retval != SQLITE_DONE) {
		cerr << "Error while creating table: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db) << endl;
	}
}

int CStateDB::rowIDofColRevEntry(int colPos, int colHash, int colRev) {
	int retval = sqlite3_bind_int(m_selectColRevStmt, 1, colPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colPos' to selectColRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectColRevStmt, 2, colHash);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colHash' to selectColRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectColRevStmt, 3, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to selectColRev statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_selectColRevStmt );
		switch(retval) {
		case SQLITE_ROW:
			rowid =	sqlite3_column_int(m_selectColRevStmt, 0);
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
		cerr << "Error stepping table columnRevs: " << sqlite3_errmsg(m_db);
	}
	retval = sqlite3_reset(m_selectColRevStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting statement: " << sqlite3_errmsg(m_db);
	}
	return rowid;
}

void CStateDB::updatePlaylistRevsTable(CSession const * const session, int minrev, int maxrev ) {
	if(minrev == -1) minrev = session->getMinPlaylistRevision();
	if(maxrev == -1) maxrev = session->getPlaylistRevision();

	for(int rev = minrev; rev < maxrev; rev++) {
		CCollection<CPlaylistItem>* playlist = session->getPlaylist(rev);

		for(int i = 0; i < playlist->size(); i++) {
			CPlaylistItem* item = playlist->getItem(i);
			updatePlaylistItem(i, item, rev, 0);
		}
	}
}

int CStateDB::rowIDofPlRevEntry(int plPos, int colHash, int plRev, int colRev) {
	int retval = sqlite3_bind_int(m_selectPlRevStmt, 1, plPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'plPos' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectPlRevStmt, 2, colHash);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colHash' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectPlRevStmt, 3, plRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'plRev' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectPlRevStmt, 4, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to selectPlRev statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_selectPlRevStmt );
		switch(retval) {
		case SQLITE_ROW:
			rowid =	sqlite3_column_int(m_selectPlRevStmt, 0);
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
	retval = sqlite3_reset(m_selectPlRevStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting statement: " << sqlite3_errmsg(m_db);
	}
	return rowid;
}

void CStateDB::updateNextlistRevsTable(CSession const * const session, int minrev, int maxrev ) {

}

void CStateDB::restoreCollection(CSession * const session) {
	if(m_db == 0) throw(CApiMisuseException("Calling restoreCollection(), but DB not opened."));

	std::string colMinRev = getValue("CollectionRevMin");
	std::string colMaxRev = getValue("CollectionRevMax");

	errno = 0;
	int minRev = strtol(colMinRev.c_str(), NULL, 10);
	assert (errno  == 0);

	errno = 0;
	int maxRev = strtol(colMaxRev.c_str(), NULL, 10);
	assert (errno  == 0);

	session->setMinCollectionRevision( minRev );

	for(int rev = minRev; rev < maxRev; rev++) {
		CCollection<CCollectionItem>* collection = new CCollection<CCollectionItem>();

		CCollectionItem* item;
		int pos = 0;
		do {
			item = getCollectionItemByPos(pos, rev);
			pos++;
			if(item) {
				collection->insert(item);
			}
		} while(item != 0);
		session->addCollectionRev(collection);
	}
}

void CStateDB::restorePlaylists(CSession * const session) {
	if(m_db == 0) throw(CApiMisuseException("Calling restorePlaylists(), but DB not opened."));

	std::string plMinRev = getValue("PlaylistRevMin");
	std::string plMaxRev = getValue("PlaylistRevMax");

	errno = 0;
	int minRev = strtol(plMinRev.c_str(), NULL, 10);
	assert (errno  == 0);

	errno = 0;
	int maxRev = strtol(plMaxRev.c_str(), NULL, 10);
	assert (errno  == 0);

	session->setMinPlaylistRevision( minRev );

	for(int rev = minRev; rev < maxRev; rev++) {
		CCollection<CPlaylistItem>* playlist = new CCollection<CPlaylistItem>();

		CPlaylistItem* item;
		int pos = 0;
		do {
			item = getPlaylistItemByPos(pos, rev);
			pos++;
			if(item) {
				playlist->insert(item);
			}
		} while(item != 0);
		session->addPlaylistRev(playlist);
	}
}

void CStateDB::restoreNextlists(CSession * const session) {
	if(m_db == 0) throw(CApiMisuseException("Calling restoreNextlists(), but DB not opened."));

	std::string nlMinRev = getValue("NextlistRevMin");
	std::string nlMaxRev = getValue("NextlistRevMax");

	errno = 0;
	int minRev = strtol(nlMinRev.c_str(), NULL, 10);
	assert (errno  == 0);

	errno = 0;
	int maxRev = strtol(nlMaxRev.c_str(), NULL, 10);
	assert (errno  == 0);

	session->setMinNextlistRevision( minRev );

	for(int rev = minRev; rev < maxRev; rev++) {
		CCollection<CPlaylistItem>* nextlist = new CCollection<CPlaylistItem>();

		CPlaylistItem* item;
		int pos = 0;
		do {
			item = getNextlistItemByPos(pos, rev);
			pos++;
			if(item) {
				nextlist->insert(item);
			}
		} while(item != 0);
		session->addNextlistRev(nextlist);
	}
}

void CStateDB::beginTansaction() throw(CApiMisuseException) {
	int retval = sqlite3_step( m_beginTransactionStmt );
	if(retval != SQLITE_DONE) {
		stringstream ss;
		ss << "Error stepping begin transaction statement: " << sqlite3_errmsg(m_db);
		throw CApiMisuseException(ss.str());
	}
}

void CStateDB::endTransaction() throw(CApiMisuseException) {
	int retval = sqlite3_step( m_endTransactionStmt );
	if(retval != SQLITE_DONE) {
		stringstream ss;
		ss << "Error stepping end transaction statement: " << sqlite3_errmsg(m_db);
		throw CApiMisuseException(ss.str());
	}
}


void CStateDB::prepareUpdateColItemStmt() {
	prepareStmt(&m_updateColItemStmt, "INSERT OR REPLACE INTO collection " \
			"( hash, file, artist, album, title, year, duration, num_played, num_skipped, num_repeated, rating) " \
			" VALUES " \
		    "(?,?,?,?,?,?,?,?,?,?,?)");
}

void CStateDB::finalizeUpdateColItemStmt() {
	finalizeStmt( &m_updateColItemStmt );
}

void CStateDB::prepareUpdateColRevStmt() {
	prepareStmt(&m_updateColRevStmt, "INSERT OR REPLACE INTO collectionRevs " \
			"( colPos, colHash, colRev) " \
			" VALUES " \
		    "(?,?,?)");
}

void CStateDB::finalizeUpdateColRevStmt() {
	finalizeStmt( &m_updateColRevStmt );
}

void CStateDB::prepareSelectColItemStmt() {
	prepareStmt(&m_selectColItemStmt, "SELECT * FROM collection WHERE hash=?");
}

void CStateDB::finalizeSelectColItemStmt() {
	finalizeStmt(&m_selectColItemStmt);
}


void CStateDB::prepareSelectColRevStmt() {
	prepareStmt(&m_selectColRevStmt, "SELECT rowid FROM collectionRevs WHERE colPos=? AND colHash=? AND colRev=?");
}

void CStateDB::finalizeSelectColRevStmt() {
	finalizeStmt( &m_selectColRevStmt );
}


void CStateDB::prepareGetColItemByPosStmt() {
	prepareStmt(&m_getColItemByPosStmt, "SELECT * FROM collection INNER JOIN collectionRevs ON collection.hash=collectionRevs.colHash WHERE collectionRevs.colPos=? AND collectionRevs.colRev=?");
}

void CStateDB::finalizeGetColItemByPosStmt() {
	finalizeStmt( &m_getColItemByPosStmt );
}

void CStateDB::prepareUpdatePlaylistItemStmt() {
	prepareStmt(&m_updatePlaylistItemStmt, "INSERT OR REPLACE INTO playlistRevs " \
			"( plPos, colHash, plRev, colRev) " \
			" VALUES " \
		    "(?,?,?,?)");
}

void CStateDB::finalizeUpdatePlaylistItemStmt() {
	finalizeStmt( &m_updatePlaylistItemStmt );
}

void CStateDB::prepareSelectPlRevStmt() {
	prepareStmt(&m_selectPlRevStmt, "SELECT rowid FROM playlistRevs WHERE plPos=? AND colHash=? AND plRev=? AND colRev=?");
}

void CStateDB::finalizeSelectPlRevStmt() {
	finalizeStmt( &m_selectPlRevStmt );
}

void CStateDB::prepareSelectPlaylistItemStmt() {
	prepareStmt(&m_selectPlaylistItemStmt, "SELECT colHash FROM playlistRevs WHERE plPos=? AND plRev=? AND colRev=?");
}

void CStateDB::finalizeSelectPlaylistItemStmt() {
	finalizeStmt( &m_selectPlaylistItemStmt );
}

void CStateDB::prepareStmt(sqlite3_stmt** stmt, std::string sqlStmt) {
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */

	int retval = sqlite3_prepare_v2(m_db, sqlStmt.c_str(), sqlStmt.size(), stmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << endl;
	}
}

void CStateDB::finalizeStmt(sqlite3_stmt** stmt) {
	int retval;
	if( *stmt ) {
		retval = sqlite3_finalize( *stmt );
		if(retval != SQLITE_OK) {
			cerr << "Error finalizing prepared statement: " << sqlite3_errmsg(m_db) << endl;
		}
		*stmt = 0;
	}
}

