/*
 * CStateDB.h
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#ifndef CSTATEDB_H_
#define CSTATEDB_H_

#include <string>
#include <sqlite3.h>

#include "CCollection.h"
#include "Exceptions.h"

class CCollectionItem;
class CPlaylistItem;
class CSession;

class CStateDB {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();

	int open();
	int close();

	std::string getValue(std::string key);
	void setValue(std::string key, std::string value);
	void setValue(std::string key, int value);

	void saveSession(CSession const * const session);
	void restoreSession(CSession * const session);

    void updateCollectionTable( CSession const * const session, int minrev = -1, int maxrev = -1 );

    void updateCollectionRevItem( int pos, int hash, int rev );

    CCollectionItem* getCollectionItemByHash(unsigned hash);
    CCollectionItem* getCollectionItemByPos(int colPos, int colRev);

    CPlaylistItem* getPlaylistItemByPos(int pos, int rev);
    CPlaylistItem* getNextlistItemByPos(int pos, int rev);

    int rowIDofColRevEntry(int colPos, int colHash, int colRev);
    int rowIDofPlRevEntry(int plPos, int colHash, int plRev, int ColRev);

    void updatePlaylistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );
    void updateNextlistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );

    void restoreCollection(CSession * const session);
    void restorePlaylists(CSession * const session);
    void restoreNextlists(CSession * const session);

private:
	std::string m_dbFileName;
    sqlite3 *m_db;

    void beginTansaction() throw(CApiMisuseException);
    void endTransaction() throw(CApiMisuseException);

    /** Revision table is very simple: rev_id , rev_nr
     *  There is a revision table for the collection, playlist and nextlist.
     */
    void createGeneralTable();
    void createCollectionTable();
    void createCollectionRevisionsTable();
    void createPlaylistRevisionsTable();
    void createNextlistRevisionsTable();

    void createTable(std::string name, std::string schema);

    void updateCollectionItem( CCollectionItem* item );
    void updatePlaylistItem( int plPos, CPlaylistItem* item, int plRev, int colRev );
    void updateNextlistItem( int nlPos, CPlaylistItem* item, int nlRev, int plRev );

    CCollectionItem* getCollectionItemFromStmt(sqlite3_stmt *pStmt);
	CPlaylistItem* getPlaylistItemFromStmt(sqlite3_stmt *pStmt);

	sqlite3_stmt *m_beginTransactionStmt;
	sqlite3_stmt *m_endTransactionStmt;

	sqlite3_stmt *m_updateColItemStmt;
	void prepareUpdateColItemStmt();
	void finalizeUpdateColItemStmt();

	sqlite3_stmt *m_updateColRevStmt;
	void prepareUpdateColRevStmt();
	void finalizeUpdateColRevStmt();

	sqlite3_stmt *m_selectColItemStmt;
	void prepareSelectColItemStmt();
	void finalizeSelectColItemStmt();

	sqlite3_stmt *m_selectColRevStmt;
	void prepareSelectColRevStmt();
	void finalizeSelectColRevStmt();

	sqlite3_stmt *m_getColItemByPosStmt;
	void prepareGetColItemByPosStmt();
	void finalizeGetColItemByPosStmt();

	sqlite3_stmt *m_updatePlaylistItemStmt;
	void prepareUpdatePlaylistItemStmt();
	void finalizeUpdatePlaylistItemStmt();

	sqlite3_stmt *m_selectPlaylistItemStmt;
	void prepareSelectPlaylistItemStmt();
	void finalizeSelectPlaylistItemStmt();

	sqlite3_stmt *m_selectPlRevStmt;
	void prepareSelectPlRevStmt();
	void finalizeSelectPlRevStmt();

	void prepareStmt(sqlite3_stmt** stmt, std::string sqlStmt);
	void finalizeStmt(sqlite3_stmt** stmt);
};

#endif /* CSTATEDB_H_ */
