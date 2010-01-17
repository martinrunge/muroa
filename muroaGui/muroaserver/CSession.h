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

class CConnection;

class CSession : public QObject{
	Q_OBJECT;
public:
	CSession();
	virtual ~CSession();

	inline CCollection<CCollectionItem>* getCollection() const { return getCollection(m_latestCollectionRevision); };
	inline CCollection<CPlaylistItem>* getPlaylist() const { return getPlaylist(m_latestPlaylistRevision); };

	CCollection<CCollectionItem>* getCollection(int revision) const;
	CCollection<CPlaylistItem>* getPlaylist(int revision) const;

	const QString getCollectionDiff(int fromRevision, int toRevision = -1);
	const QString getPlaylistDiff(int fromRevision, int toRevision = -1);

	inline int getCollectionRevision() { return m_latestCollectionRevision; };
	inline int getPlaylistRevision() { return m_latestPlaylistRevision; };

	void addCollectionRev(QString collection);
	void addPlaylistRev(QString playlist);

	int addCollectionRevFromDiff(const QString& collectionDiff, int diffFromRev);
	int addPlaylistRevFromDiff(const QString& playlistDiff, int diffFromRev);

	void addConnection(CConnection* connection);

public slots:
	void connectionClosed(CConnection* conn);

private:
	QMap<int, CCollection<CCollectionItem>* > m_collectionRevisions;
	QMap<int, CCollection<CPlaylistItem>* > m_playlistRevisions;

	QList<CConnection*> m_connections;

	int m_latestCollectionRevision;
	int m_latestPlaylistRevision;

	QString m_name;
};

#endif /* CSESSION_H_ */
