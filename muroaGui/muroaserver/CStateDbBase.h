/*
 * CStateDbBase.h
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#ifndef CSTATEDBBASE_H_
#define CSTATEDBBASE_H_

#include <string>
#include <sqlite3.h>

#include "CCollection.h"
#include "Exceptions.h"

class CMediaItem;
class CPlaylistItem;
class CSession;

class CStateDbBase {
public:
	CStateDbBase(std::string dbFileName);
	virtual ~CStateDbBase();

	virtual int open();
	virtual int close();

	std::string getValue(std::string key);
	void setValue(std::string key, std::string value);
	void setValue(std::string key, int value);

    void updateCollectionRevItem( int pos, int hash, int rev );

    CMediaItem* getMediaItemByHash(unsigned hash);
    CMediaItem* getMediaItemByPos(int colPos, int colRev);

    int rowIDofColRevEntry(int colPos, int colHash, int colRev);

protected:
    sqlite3 *m_db;

    void updateMediaItem( CMediaItem* item );

    void beginTansaction() throw(CApiMisuseException);
    void endTransaction() throw(CApiMisuseException);

	void prepareStmt(sqlite3_stmt** stmt, std::string sqlStmt);
	void finalizeStmt(sqlite3_stmt** stmt);

private:
	std::string m_dbFileName;

    /** Revision table is very simple: rev_id , rev_nr
     *  There is a revision table for the collection, playlist and nextlist.
     */
    void createGeneralTable();
    void createCollectionTable();
    void createCollectionRevisionsTable();
    void createPlaylistRevisionsTable();
    void createNextlistRevisionsTable();

    void createTable(std::string name, std::string schema);

    CMediaItem* getMediaItemFromStmt(sqlite3_stmt *pStmt);


	sqlite3_stmt *m_beginTransactionStmt;
	sqlite3_stmt *m_endTransactionStmt;

	sqlite3_stmt *m_updateMediaItemStmt;
	void prepareUpdateMediaItemStmt();
	void finalizeUpdateMediaItemStmt();

	sqlite3_stmt *m_updateColRevStmt;
	void prepareUpdateColRevStmt();
	void finalizeUpdateColRevStmt();

	sqlite3_stmt *m_selectMediaItemStmt;
	void prepareSelectMediaItemStmt();
	void finalizeSelectMediaItemStmt();


	sqlite3_stmt *m_selectColRevStmt;
	void prepareSelectColRevStmt();
	void finalizeSelectColRevStmt();



};

#endif /* CSTATEDBBASE_H_ */
