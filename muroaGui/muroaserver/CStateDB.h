/*
 * CStateDB.h
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#ifndef CSTATEDB_H_
#define CSTATEDB_H_

#include "CStateDbBase.h"

class CCollectionItem;
class CPlaylistItem;
class CSession;


class CStateDB: public CStateDbBase {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();

	int open();
	int close();


	void saveSession(CSession const * const session);
	void restoreSession(CSession * const session);

    CCollectionItem* getCollectionItemByHash(unsigned hash);
    CCollectionItem* getCollectionItemByPos(int colPos, int colRev);

    CPlaylistItem* getPlaylistItemByPos(int pos, int rev);
    CPlaylistItem* getNextlistItemByPos(int pos, int rev);

    void updateCollectionTable( CSession const * const session, int minrev = -1, int maxrev = -1 );


    void updatePlaylistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );
    void updateNextlistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );

    void restoreCollection(CSession * const session);
    void restorePlaylists(CSession * const session);
    void restoreNextlists(CSession * const session);



private:
    CCollectionItem* getCollectionItemFromStmt(sqlite3_stmt *pStmt);
	CPlaylistItem* getPlaylistItemFromStmt(sqlite3_stmt *pStmt);

    void updateCollectionItem( CCollectionItem* item );

    void updatePlaylistItem( int plPos, CPlaylistItem* item, int plRev, int colRev );
    void updateNextlistItem( int nlPos, CPlaylistItem* item, int nlRev, int plRev );

    int rowIDofPlRevEntry(int plPos, int colHash, int plRev, int ColRev);

	sqlite3_stmt *m_updateColItemStmt;
	void prepareUpdateColItemStmt();
	void finalizeUpdateColItemStmt();

	sqlite3_stmt *m_selectColItemStmt;
	void prepareSelectColItemStmt();
	void finalizeSelectColItemStmt();

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

};

#endif /* CSTATEDB_H_ */
