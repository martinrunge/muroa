/*
 * CSession.h
 *
 *  Created on: 6 Nov 2009
 *      Author: martin
 */

#ifndef CSESSION_H_
#define CSESSION_H_

#include <QObject>
#include <QMap>
#include <QString>

#include "CCollectionItem.h"
#include "CPlaylistItem.h"
#include "CCollection.h"
#include "CStateDB.h"

#include "Exceptions.h"

#include "mediaprocessing/CStream.h"

class CConnection;

class CSession : public QObject{
	Q_OBJECT;
public:
	CSession();
	virtual ~CSession();

	inline CCollection<CCollectionItem>* getCollection() const { return getCollection(m_latestCollectionRevision); };
	inline CCollection<CPlaylistItem>* getPlaylist() const { return getPlaylist(m_latestPlaylistRevision); };
	inline CCollection<CPlaylistItem>* getNextlist() const { return getNextlist(m_latestNextlistRevision); };

	CCollection<CCollectionItem>* getCollection(int revision) const;
	CCollection<CPlaylistItem>* getPlaylist(int revision) const;
	CCollection<CPlaylistItem>* getNextlist(int revision) const;

	const QString getCollectionDiff(int fromRevision, int toRevision = -1);
	const QString getPlaylistDiff(int fromRevision, int toRevision = -1);
	const QString getNextlistDiff(int fromRevision, int toRevision = -1);

	inline int getCollectionRevision() const { return m_latestCollectionRevision; };
	inline int getPlaylistRevision() const { return m_latestPlaylistRevision; };
	inline int getNextlistRevision() const { return m_latestNextlistRevision; };

	inline int getMinCollectionRevision() const { return m_minCollectionRevision; };
	inline int getMinPlaylistRevision() const { return m_minPlaylistRevision; };
	inline int getMinNextlistRevision() const { return m_minNextlistRevision; };


	void addCollectionRev(CCollection<CCollectionItem>* collection);
	void addCollectionRev(QString collection);
	void addPlaylistRev(CCollection<CPlaylistItem>* playlist);
	void addPlaylistRev(QString playlist);
	void addNextlistRev(CCollection<CPlaylistItem>* nextlist);
	void addNextlistRev(QString playlist);

	int addCollectionRevFromDiff(QString* collectionDiff, int diffFromRev) throw(InvalidMsgException);
	int addPlaylistRevFromDiff(QString* playlistDiff, int diffFromRev) throw(InvalidMsgException);
	int addNextlistRevFromDiff(QString* nextlistDiff, int diffFromRev) throw(InvalidMsgException);

	int addNextlistRevFromNextCmd();
	int addNextlistRevFromPrevCmd();

	void setMinCollectionRevision(int rev) throw();
	void setMinPlaylistRevision(int rev) throw();
	void setMinNextlistRevision(int rev) throw();

	void addConnection(CConnection* connection);

public slots:
	void play();
	void stop();
	void next();
	void prev();

	void connectionClosed(CConnection* conn);
	void progress(int done, int total);

	void scanProgress(int donw, int total);

	void saveState();
	void restoreState();

private:
	CStream m_stream;

	QMap<int, CCollection<CCollectionItem>* > m_collectionRevisions;
	QMap<int, CCollection<CPlaylistItem>* > m_playlistRevisions;
	QMap<int, CCollection<CPlaylistItem>* > m_nextlistRevisions;

	QList<CConnection*> m_connections;

	void setSong();

	int m_latestCollectionRevision;
	int m_latestPlaylistRevision;
	int m_latestNextlistRevision;

	int m_minCollectionRevision;
	int m_minPlaylistRevision;
	int m_minNextlistRevision;

	int m_playlistPos;

	CStateDB m_stateDB;
	QString m_name;
};

#endif /* CSESSION_H_ */
