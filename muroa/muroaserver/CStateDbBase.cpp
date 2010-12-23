/*
 * CStateDbBase.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CStateDbBase.h"
#include "mediascanner/CMediaItem.h"

#include <iostream>
#include <sstream>

#include <assert.h>

using namespace std;

CStateDbBase::CStateDbBase(std::string dbFileName) : m_dbFileName( dbFileName ),
		                                     m_db(0),
		                                     m_beginTransactionStmt(0),
		                                     m_endTransactionStmt(0),
		                                     m_updateColRevStmt(0),
		                                     m_selectColRevStmt(0),
		                                     m_selectMediaItemStmt(0),
		                                     m_updateMediaItemStmt(0),
		                                     m_getMediaItemByPosStmt(0)
{
}

CStateDbBase::~CStateDbBase() {
	close();
}


int CStateDbBase::open() {
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

		prepareGetMediaItemByPosStmt();
		prepareUpdateMediaItemStmt();
		prepareUpdateColRevStmt();
		prepareSelectColRevStmt();

	}
	return rc;
}

int CStateDbBase::close() {

	finalizeUpdateColRevStmt();
    finalizeSelectColRevStmt();
    finalizeUpdateMediaItemStmt();
	prepareGetMediaItemByPosStmt();

	finalizeStmt(&m_beginTransactionStmt);
	finalizeStmt(&m_endTransactionStmt);

	if(m_db != 0) {
		sqlite3_close(m_db);
		m_db = 0;
	}

	return 0;
}

std::string CStateDbBase::getValue(std::string key) {
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

int CStateDbBase::getIntValue(std::string key) {
	std::string asString = getValue(key);
	int retval;

	std::istringstream stream(asString);
    stream >> retval;
	return retval;
}

void CStateDbBase::setValue(std::string key, std::string value) {
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

void CStateDbBase::setValue(std::string key, int value) {
	stringstream ss;
	ss << value;
	setValue(key, ss.str());
}

void CStateDbBase::createGeneralTable() {
	createTable("general", "(key TEXT UNIQUE, value TEXT)");
}

void CStateDbBase::createCollectionTable( ) {
	createTable("collection" , "(hash INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, title TEXT, year INTEGER, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)");
}

void CStateDbBase::createCollectionRevisionsTable() {
	createTable("collectionRevs" , "(colPos INTEGER, colHash INTEGER REFERENCES collection (hash),  ColRev INTEGER)");
}

void CStateDbBase::updateMediaItem( CMediaItem* item ) {
	assert(m_updateMediaItemStmt != 0);

	int retval = sqlite3_bind_int(m_updateMediaItemStmt, 1, item->getHash());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'hash' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 2, item->getFilename().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'filename' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 3, item->getArtist().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'Artist' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 4, item->getAlbum().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'album' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_text(m_updateMediaItemStmt, 5, item->getTitle().c_str(), -1, SQLITE_TRANSIENT);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'title' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 6, item->getYear());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'year' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 7, item->getDuration());
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'duration' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 8, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_played' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 9, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_skipped' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 10, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'num_repeated' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_updateMediaItemStmt, 11, 0);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'rating' to updateMediaItemStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	retval = sqlite3_step( m_updateMediaItemStmt );

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping m_updateMediaItemStmt: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_reset(m_updateMediaItemStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting m_updateMediaItemStmt statement: " << sqlite3_errmsg(m_db);
	}
}

CMediaItem* CStateDbBase::getMediaItemByPos(int colPos, int colRev) {
	CMediaItem* item = 0;

	assert(m_getMediaItemByPosStmt != 0);

	int retval = sqlite3_bind_int(m_getMediaItemByPosStmt, 1, colPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colPos' to getMediaItemByPosStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_getMediaItemByPosStmt, 2, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to getMediaItemByPosStmt statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_getMediaItemByPosStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getMediaItemFromStmt(m_getMediaItemByPosStmt);
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
		cerr << "Error stepping getMediaItemByPos: " << sqlite3_errmsg(m_db);
	}
	retval = sqlite3_reset(m_getMediaItemByPosStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting getMediaItemByPos statement: " << sqlite3_errmsg(m_db);
	}
	return item;
}



void CStateDbBase::updateCollectionRevItem( int pos, int hash, int rev ) {

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




CMediaItem* CStateDbBase::getMediaItemFromStmt(sqlite3_stmt *pStmt) {
	CMediaItem* item = new CMediaItem;
	// (hash INTEGER, file TEXT, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	int numCol = sqlite3_column_count(pStmt);
	// Scerr << "result has " << numCol << " columns" << endl;

	int hash = sqlite3_column_int(pStmt, 0);

	const unsigned char *filename = sqlite3_column_text(pStmt, 1);
	int filenameSize = sqlite3_column_bytes(pStmt, 1);
	item->setFilename( (const char*) filename );

	const unsigned char *artist = sqlite3_column_text(pStmt, 2);
	int artistSize = sqlite3_column_bytes(pStmt, 2);
	item->setArtist( (const char*)artist );

	const unsigned char *album = sqlite3_column_text(pStmt, 3);
	int albumSize = sqlite3_column_bytes(pStmt, 3);
	item->setAlbum( (const char*) album );

	const unsigned char *title = sqlite3_column_text(pStmt, 4);
	int titleSize = sqlite3_column_bytes(pStmt, 4);
	item->setTitle( (const char*) title );

	int year = sqlite3_column_int(pStmt, 5);
	item->setYear( year );

	int duration = sqlite3_column_int(pStmt, 6);
	item->setDuration( duration );

//	cerr << "SELECT: " << filename << " " << artist << " " << album << " " << title << endl;
	return item;
}



/** \brief  Create a database table if it does not yet exist
 *
 *
 * \param   name   Name of the table to create
 * \param   schema Table schema. e.g. "(rev_id INTEGER, rev INTEGER)"
 * \exception  todexception on failure
 */
void CStateDbBase::createTable(std::string name, std::string schema) {
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

int CStateDbBase::rowIDofColRevEntry(int colPos, int colHash, int colRev) {
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




void CStateDbBase::beginTansaction() throw(CApiMisuseException) {
	int retval = sqlite3_step( m_beginTransactionStmt );
	if(retval != SQLITE_DONE) {
		stringstream ss;
		ss << "Error stepping begin transaction statement: " << sqlite3_errmsg(m_db);
		throw CApiMisuseException(ss.str());
	}
}

void CStateDbBase::endTransaction() throw(CApiMisuseException) {
	int retval = sqlite3_step( m_endTransactionStmt );
	if(retval != SQLITE_DONE) {
		stringstream ss;
		ss << "Error stepping end transaction statement: " << sqlite3_errmsg(m_db);
		throw CApiMisuseException(ss.str());
	}
}

void CStateDbBase::prepareUpdateMediaItemStmt() {
	prepareStmt(&m_updateMediaItemStmt, "INSERT OR REPLACE INTO collection " \
			"( hash, file, artist, album, title, year, duration, num_played, num_skipped, num_repeated, rating) " \
			" VALUES " \
		    "(?,?,?,?,?,?,?,?,?,?,?)");
}

void CStateDbBase::finalizeUpdateMediaItemStmt() {
	finalizeStmt( &m_updateMediaItemStmt );
}

void CStateDbBase::prepareUpdateColRevStmt() {
	prepareStmt(&m_updateColRevStmt, "INSERT OR REPLACE INTO collectionRevs " \
			"( colPos, colHash, colRev) " \
			" VALUES " \
		    "(?,?,?)");
}

void CStateDbBase::finalizeUpdateColRevStmt() {
	finalizeStmt( &m_updateColRevStmt );
}

void CStateDbBase::prepareGetMediaItemByPosStmt() {
	prepareStmt(&m_getMediaItemByPosStmt, "SELECT * FROM collection INNER JOIN collectionRevs ON collection.hash=collectionRevs.colHash WHERE collectionRevs.colPos=? AND collectionRevs.colRev=?");
}

void CStateDbBase::finalizeGetMediaItemByPosStmt() {
	finalizeStmt( &m_getMediaItemByPosStmt );
}


void CStateDbBase::prepareSelectMediaItemStmt() {
	prepareStmt(&m_selectMediaItemStmt, "SELECT * FROM collection WHERE hash=?");
}

void CStateDbBase::finalizeSelectMediaItemStmt() {
	finalizeStmt(&m_selectMediaItemStmt);
}


void CStateDbBase::prepareSelectColRevStmt() {
	prepareStmt(&m_selectColRevStmt, "SELECT rowid FROM collectionRevs WHERE colPos=? AND colHash=? AND colRev=?");
}

void CStateDbBase::finalizeSelectColRevStmt() {
	finalizeStmt( &m_selectColRevStmt );
}



void CStateDbBase::prepareStmt(sqlite3_stmt** stmt, std::string sqlStmt) {
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */

	int retval = sqlite3_prepare_v2(m_db, sqlStmt.c_str(), sqlStmt.size(), stmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << endl;
	}
}

void CStateDbBase::finalizeStmt(sqlite3_stmt** stmt) {
	int retval;
	if( *stmt ) {
		retval = sqlite3_finalize( *stmt );
		if(retval != SQLITE_OK) {
			cerr << "Error finalizing prepared statement: " << sqlite3_errmsg(m_db) << endl;
		}
		*stmt = 0;
	}
}

