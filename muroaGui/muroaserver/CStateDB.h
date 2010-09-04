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
class CCollectionItem;
class CSession;

class CStateDB {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();

	int open();
	int close();

	std::string getValue(std::string key);
	void setValue(std::string key, std::string value);

	void saveSession(CSession const * const session);
	void restoreSession(CSession const * session);

    void updateCollectionDB( CSession const * const session, int minrev = -1, int maxrev = -1 );
    void updateCollectionTable( CCollection<CCollectionItem>* collection );

    void updateCollectionRevItem( int pos, int hash, int rev );

    CCollectionItem* getCollectionItemByHash(unsigned hash);
    CCollectionItem* getCollectionItemByPos(int colPos, int colRev);

    int rowIDofColRevEntry(int colPos, int colHash, int colRev);

    void updatePlaylistsTable(CSession const * const session);
    void updateNextlistsTable(CSession const * const session);

    void restoreCollection(CSession const * session);
    void restorePlaylists(CSession const * session);
    void restoreNextlists(CSession const * session);

private:
	std::string m_dbFileName;
    sqlite3 *m_db;

    /** Revision table is very simple: rev_id , rev_nr
     *  There is a revision table for the collection, playlist and nextlist.
     */
    void createGeneralTable();
    void createCollectionTable();
    void createCollectionRevisionsTable();
    void createPlaylistsTable();
    void createNextlistsTable();

    void createTable(std::string name, std::string schema);

    void updateCollectionItem( CCollectionItem* item );

    CCollectionItem* getCollectionItemFromStmt(sqlite3_stmt *pStmt);

	sqlite3_stmt *m_selectColRevStmt;
	void prepareSelectColRevStmt();
	void finalizeSelectColRevStmt();

	sqlite3_stmt *m_getColItemByPosStmt;
	void prepareGetColItemByPosStmt();
	void finalizeGetColItemByPosStmt();

	void prepareStmt(sqlite3_stmt** stmt, std::string sqlStmt);
	void finalizeStmt(sqlite3_stmt** stmt);
};

#endif /* CSTATEDB_H_ */
