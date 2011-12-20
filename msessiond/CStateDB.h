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
	int close();	void saveSession(CSession const * const session);
	void restoreSession(CSession * const session);

	CRootItem* getMediaColRev(int rev);

	CMediaItem* getMediaItemByHash(unsigned hash);
    CMediaItem* getMediaItemByPos(int colPos, int colRev);

    CPlaylistItem* getPlaylistItemByPos(int pos, int rev);
    CNextlistItem* getNextlistItemByPos(int pos, int rev);

    void updateMediaColTable( CSession const * const session, int minrev = -1, int maxrev = -1 );

    void updatePlaylistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );
    void updateNextlistRevsTable(CSession const * const session, int minrev = -1, int maxrev = -1 );

    void restoreMediaCols(CSession * const session);
    void restorePlaylists(CSession * const session);
    void restoreNextlists(CSession * const session);



private:
    CMediaItem* getMediaItemFromStmt(sqlite3_stmt *pStmt);
	CPlaylistItem* getPlaylistItemFromStmt(sqlite3_stmt *pStmt);
	CNextlistItem* getNextlistItemFromStmt(sqlite3_stmt *pStmt);

    void updateMediaItem( CMediaItem* item );

    void updatePlaylistItem( int plPos, CPlaylistItem* item, int plRev, int colRev );
    void updateNextlistItem( int nlPos, CPlaylistItem* item, int nlRev, int plRev );

    void createPlaylistRevisionsTable();
    void createNextlistRevisionsTable();

    int rowIDofPlRevEntry(int plPos, int colHash, int plRev, int ColRev);
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
};

}
#endif /* CSTATEDB_H_ */
