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

	inline int getCollectionRevision() { return m_latestCollectionRevision; };
	inline int getPlaylistRevision() { return m_latestPlaylistRevision; };
	inline int getNextlistRevision() { return m_latestNextlistRevision; };

	void addCollectionRev(QString collection);
	void addPlaylistRev(QString playlist);
	void addNextlistRev(QString playlist);

	int addCollectionRevFromDiff(QString* collectionDiff, int diffFromRev);
	int addPlaylistRevFromDiff(QString* playlistDiff, int diffFromRev);
	int addNextlistRevFromDiff(QString* nextlistDiff, int diffFromRev);

	void addConnection(CConnection* connection);

public slots:
	void play();
	void stop();
	void next();
	void prev();

	void connectionClosed(CConnection* conn);

	void progress(int done, int total);

private:
	CStream m_stream;

	QMap<int, CCollection<CCollectionItem>* > m_collectionRevisions;
	QMap<int, CCollection<CPlaylistItem>* > m_playlistRevisions;
	QMap<int, CCollection<CPlaylistItem>* > m_nextlistRevisions;

	QList<CConnection*> m_connections;

	int m_latestCollectionRevision;
	int m_latestPlaylistRevision;
	int m_latestNextlistRevision;

	QString m_name;
};

#endif /* CSESSION_H_ */
