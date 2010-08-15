/*
 * CStateDB.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CStateDB.h"

#include <iostream>
#include <sstream>

#include <QDebug>

using namespace std;

CStateDB::CStateDB(std::string dbFileName) : m_dbFileName( dbFileName ), m_db(0) {
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
		createGeneralTable();
		createCollectionTable();
		createPlaylistsTable();
		createNextlistsTable();
	}
}

int CStateDB::close() {
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

	cerr << "SQL statement: " << sql_stmt << endl;
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

void CStateDB::createGeneralTable() {
	createTable("general", "(key TEXT UNIQUE, value TEXT)");
}

void CStateDB::createCollectionTable( ) {
	createTable("collection" , "(song_id INTEGER PRIMARY KEY AUTOINCREMENT, file TEXT, hash INTEGER, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)");
}

void CStateDB::createPlaylistsTable() {
	createTable("playlists","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, pl_pos INTEGER, pl_rev INTEGER, col_id INTEGER, FOREIGN KEY(col_id) REFERENCES collection(song_id))");
}

void CStateDB::createNextlistsTable() {
	createTable("nextlists","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, nl_pos INTEGER, nl_rev INTEGER, pl_id INTEGER, FOREIGN KEY(pl_id) REFERENCES playlists(entry_id))");
}


void CStateDB::updateCollectionDB( CCollection<CCollectionItem>* collection ) {
	for(int i = 0; i < collection->size(); i++) {
		CCollectionItem* item = collection->getItem(i);
		updateCollectionItem(item);
	}
}

void CStateDB::updateCollectionItem( CCollectionItem* item ) {
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */
	static int id = 0;
	id++;
	stringstream ss;
	// (song_id INTEGER, file TEXT, hash INTEGER, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	ss << "INSERT OR REPLACE INTO collection "
	   << "( file, hash, artist, album, title, duration, num_played, num_skipped, num_repeated, rating)"
	   << " VALUES "
	   << "('" << item->getFilename().toUtf8().data() << "','" << item->getHash() << "','" << item->getArtist().toUtf8().data() << "','" << item->getAlbum().toUtf8().data() << "','" << item->getTitle().toUtf8().data() << "','" << item->getLengthInSec() << "','"<< 0 <<"','" << 0 << "','"<< 0 <<"','" << 0 << "') ";
	string sql_stmt = ss.str();

	cerr << "file: " << item->getFilename().toUtf8().data() << endl;
	cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	do {
		retval = sqlite3_step( pStmt );
	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}


unsigned CStateDB::getSongIdByHash(unsigned hash) {
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */
	stringstream ss;
	// (song_id INTEGER, file TEXT, hash INTEGER, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	ss << "SELECT * FROM collection WHERE hash='" << hash << "'";
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
			item = getItemFromStmt(pStmt);
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
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}

CCollectionItem* CStateDB::getItemFromStmt(sqlite3_stmt *pStmt) {
	CCollectionItem* item = new CCollectionItem;
	// (song_id INTEGER, file TEXT, hash INTEGER, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	int numCol = sqlite3_column_count(pStmt);
	cerr << "result has " << numCol << " columns" << endl;

	int song_id = sqlite3_column_int(pStmt, 0);

	const unsigned char *filename = sqlite3_column_text(pStmt, 1);
	int filenameSize = sqlite3_column_bytes(pStmt, 1);

	int hash = sqlite3_column_int(pStmt, 2);

	const unsigned char *artist = sqlite3_column_text(pStmt, 3);
	int artistSize = sqlite3_column_bytes(pStmt, 3);

	const unsigned char *album = sqlite3_column_text(pStmt, 4);
	int albumSize = sqlite3_column_bytes(pStmt, 4);

	const unsigned char *title = sqlite3_column_text(pStmt, 5);
	int titleSize = sqlite3_column_bytes(pStmt, 5);

	cerr << "SELECT: " << filename << " " << artist << " " << album << " " << title << endl;

	return item;
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

	cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_step( pStmt );

	if(retval != SQLITE_DONE) {
		cerr << "Error while creating table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}
