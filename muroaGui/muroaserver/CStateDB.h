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

class CStateDB {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();

	int open();
	int close();

	std::string getValue(std::string key);
	void setValue(std::string key, std::string value);

    void updateCollectionDB( CCollection<CCollectionItem>* collection ) ;
    unsigned getSongIdByHash(unsigned hash);

private:
	std::string m_dbFileName;
    sqlite3 *m_db;

    /** Revision table is very simple: rev_id , rev_nr
     *  There is a revision table for the collection, playlist and nextlist.
     */
    void createGeneralTable();
    void createCollectionTable();
    void createPlaylistsTable();
    void createNextlistsTable();

    void createTable(std::string name, std::string schema);

    void updateCollectionItem( CCollectionItem* item );

    CCollectionItem* getItemFromStmt(sqlite3_stmt *pStmt);

};

#endif /* CSTATEDB_H_ */
