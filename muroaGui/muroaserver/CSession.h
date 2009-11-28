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

class CConnection;
class CPlaylistItem;

class CSession : public QObject{
	Q_OBJECT;
public:
	CSession();
	virtual ~CSession();

	inline const QString getCollection() const { return getCollection(m_latestCollectionRevision); };
	inline const QString getPlaylist() const { return getPlaylist(m_latestPlaylistRevision); };

	const QString getCollection(int revision) const;
	const QString getPlaylist(int revision) const;

	const QString getCollectionDiff(int fromRevision, int toRevision = -1);

	inline int getCollectionRevision() { return m_latestCollectionRevision; };
	inline int getPlaylistRevision() { return m_latestPlaylistRevision; };


	void addCollectionRev(const QString collection);
	void addPlaylistRev(const QString playlist);

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
