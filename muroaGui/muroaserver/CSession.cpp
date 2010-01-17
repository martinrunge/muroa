/*
 * CSession.cpp
 *
 *  Created on: 6 Nov 2009
 *      Author: martin
 */

#include "CSession.h"
#include "CDiff.h"
#include "cconnection.h"


CSession::CSession() : m_latestCollectionRevision(0),
                       m_latestPlaylistRevision(0)
{
	// TODO Auto-generated constructor stub

}

CSession::~CSession() {
	// TODO Auto-generated destructor stub
}

CCollection<CCollectionItem>* CSession::getCollection(int revision) const
{
	if(m_collectionRevisions.contains(revision))
	{
		return m_collectionRevisions.value(revision);
	}
	else
	{
		return 0;
	}
}

CCollection<CPlaylistItem>* CSession::getPlaylist(int revision) const
{
	if(m_playlistRevisions.contains(revision))
	{
		return m_playlistRevisions.value(revision);
	}
	else
	{
		return 0;
	}
}


const QString CSession::getCollectionDiff(int fromRevision, int toRevision)
{
	if(toRevision == -1)
	{
		toRevision = m_latestCollectionRevision;
	}

	if(fromRevision == -1
	   || ! m_collectionRevisions.contains(fromRevision)
	   || ! m_collectionRevisions.contains(toRevision))
	{
		return QString();
	}

	CDiff differ;

	QString knownRev = m_collectionRevisions.value(fromRevision)->getText();
	QString targetRev = m_collectionRevisions.value(toRevision)->getText();
	QString diffString = differ.diff(knownRev, targetRev);

	return diffString;
}


const QString CSession::getPlaylistDiff(int fromRevision, int toRevision)
{
	if(toRevision == -1)
	{
		toRevision = m_latestPlaylistRevision;
	}

	if(fromRevision == -1
	   || ! m_playlistRevisions.contains(fromRevision)
	   || ! m_playlistRevisions.contains(toRevision))
	{
		return QString();
	}

	CDiff differ;

	QString knownRev = m_playlistRevisions.value(fromRevision)->getText();
	QString targetRev = m_playlistRevisions.value(toRevision)->getText();
	QString diffString = differ.diff(knownRev, targetRev);

	return diffString;
}


void CSession::addCollectionRev(QString collection)
{
	m_latestCollectionRevision++;
	CCollection<CCollectionItem>* newCollection = new CCollection<CCollectionItem>();
	newCollection->setText(collection, m_latestCollectionRevision);

	qDebug() << newCollection->getText();

	m_collectionRevisions[m_latestCollectionRevision] = newCollection;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendCollection(m_latestCollectionRevision - 1);
	}
}

void CSession::addPlaylistRev(QString playlist)
{
	m_latestPlaylistRevision++;
	CCollection<CPlaylistItem>* newPlaylist = new CCollection<CPlaylistItem>();
	newPlaylist->setText(playlist, m_latestPlaylistRevision);

	qDebug() << newPlaylist->getText();

	m_playlistRevisions[m_latestPlaylistRevision] = newPlaylist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendPlaylist(m_latestPlaylistRevision - 1);
	}
}


int CSession::addCollectionRevFromDiff(const QString& collectionDiff, int diffFromRev)
{
	CCollection<CCollectionItem>* newCollection = new CCollection<CCollectionItem>( *(getCollection(m_latestCollectionRevision)) );
	newCollection->patch(collectionDiff, ++m_latestCollectionRevision);

	qDebug() << newCollection->getText();

	m_collectionRevisions[m_latestCollectionRevision] = newCollection;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendCollection(m_latestCollectionRevision - 1);
	}

}

int CSession::addPlaylistRevFromDiff(const QString& playlistDiff, int diffFromRev)
{
	qDebug() << QString("CSession::addPlaylistRevFromDiff %1 %2").arg(playlistDiff).arg(diffFromRev);
	CCollection<CPlaylistItem>* newPlaylist = new CCollection<CPlaylistItem>( *(getPlaylist(m_latestPlaylistRevision)) );
	newPlaylist->patch(playlistDiff, ++m_latestPlaylistRevision);

	qDebug() << newPlaylist->getText();

	m_playlistRevisions[m_latestPlaylistRevision] = newPlaylist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendPlaylist(m_latestPlaylistRevision - 1);
	}
}



void CSession::addConnection(CConnection* connection)
{
	if(!m_connections.contains(connection))
	{
		// avoid duplicates
		m_connections.append(connection);
		connection->setSessionPtr(this);
	}
}

void CSession::connectionClosed(CConnection* conn)
{
	//if(m_connections.contains(conn))
	//{
	m_connections.removeAll(conn);
	//}
}
