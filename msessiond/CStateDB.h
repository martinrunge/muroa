/*
 * CStateDB.h
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#ifndef CSTATEDB_H_
#define CSTATEDB_H_

#include "CStateDbBase.h"

#include "CRootItem.h"

class CMediaItem;
class CPlaylistItem;
class CNextlistItem;

namespace muroa {

class CSession;

class CStateDB: public CStateDbBase {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();

	int open();
	int close();
	void saveSession(CSession const * const session);
	void restoreSession(CSession * const session);

	CRootItem* getMediaColRev(int rev);

	CMediaItem* getMediaItemByHash(unsigned hash, CRootItem* ri);
    CMediaItem* getMediaItemByPos(int colPos, int colRev, CRootItem* ri);

    CPlaylistItem* getPlaylistItemByPos(int pos, int rev, CRootItem* ri);
    CNextlistItem* getNextlistItemByPos(int pos, int rev, CRootItem* ri);

    void updateMediaColTable( CSession const * const session, int minrev = -1, int maxrev = -1 );

    void updatePlaylistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );
    void updateNextlistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );

    void restoreMediaCols(CSession * const session);
    void restorePlaylists(CSession * const session);
    void restoreNextlists(CSession * const session);



private:
    CMediaItem* getMediaItemFromStmt(sqlite3_stmt *pStmt, CRootItem* ri);
	CPlaylistItem* getPlaylistItemFromStmt(sqlite3_stmt *pStmt, CRootItem* ri);
	CNextlistItem* getNextlistItemFromStmt(sqlite3_stmt *pStmt, CRootItem* ri);

    void updateMediaItem( CMediaItem* item );

    void updatePlaylistItem( int plPos, CPlaylistItem* item, int plRev, int colRev );
    void updateNextlistItem( int nlPos, CNextlistItem* item, int nlRev, int plRev, int colRev );

    void createPlaylistRevisionsTable();
    void createNextlistRevisionsTable();

    void createRevisionMatchTable();

    int rowIDofPlRevEntry(int plPos, int colHash, int plRev, int ColRev);
    int rowIDofNlRevEntry(int nlPos, int colHash, int plHash, int nlRev, int plRev, int ColRev);
    void repairSession(CSession* session);

	sqlite3_stmt *m_updateMediaItemStmt;
	void prepareUpdateMediaItemStmt();
	void finalizeUpdateMediaItemStmt();

	sqlite3_stmt *m_selectMediaItemStmt;
	void prepareSelectMediaItemStmt();
	void finalizeSelectMediaItemStmt();

	sqlite3_stmt *m_getMediaItemByPosStmt;
	void prepareGetMediaItemByPosStmt();
	void finalizeGetMediaItemByPosStmt();

	sqlite3_stmt *m_updatePlaylistItemStmt;
	void prepareUpdatePlaylistItemStmt();
	void finalizeUpdatePlaylistItemStmt();

	sqlite3_stmt *m_selectPlaylistItemStmt;
	void prepareSelectPlaylistItemStmt();
	void finalizeSelectPlaylistItemStmt();

	sqlite3_stmt *m_selectPlaylistRevStmt;
	void prepareSelectPlaylistRevStmt();
	void finalizeSelectPlaylistRevStmt();

	sqlite3_stmt *m_updateNextlistItemStmt;
	void prepareUpdateNextlistItemStmt();
	void finalizeUpdateNextlistItemStmt();

	sqlite3_stmt *m_selectNextlistItemStmt;
	void prepareSelectNextlistItemStmt();
	void finalizeSelectNextlistItemStmt();

	sqlite3_stmt *m_selectNextlistRevStmt;
	void prepareSelectNextlistRevStmt();
	void finalizeSelectNextlistRevStmt();

	sqlite3_stmt *m_updateRevMatchStmt;
	void prepareUpdateRevMatchStmt();
	void finalizeUpdateRevMatchStmt();

	sqlite3_stmt *m_colRevForPlRevStmt;
	void prepareColRevForPlRevStmt();
	void finmalizeColRevForPlRevStmt();
	unsigned getColRevForPlRev(unsigned plRev);

	sqlite3_stmt *m_colRevForNlRevStmt;
	unsigned getColRevForNlRev(unsigned nlRev);
};

}
#endif /* CSTATEDB_H_ */
