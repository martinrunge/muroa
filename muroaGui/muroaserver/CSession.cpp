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

const QString  CSession::getCollection(int revision) const
{
	if(m_collectionRevisions.contains(revision))
	{
		return m_collectionRevisions.value(revision);
	}
	else
	{
		return QString();
	}
}

const QString CSession::getPlaylist(int revision) const
{
	if(m_playlistRevisions.contains(revision))
	{
		return m_playlistRevisions.value(revision);
	}
	else
	{
		return QString();
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

	QString knownRev = m_collectionRevisions.value(fromRevision);
	QString targetRev = m_collectionRevisions.value(toRevision);
	QString diffString = differ.diff(knownRev, targetRev);

	return diffString;
}


void CSession::addCollectionRev(const QString collection)
{
	m_latestCollectionRevision++;
	m_collectionRevisions[m_latestCollectionRevision] = collection;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendCollection(m_latestCollectionRevision - 1);
	}
}

void CSession::addPlaylistRev(const QString playlist)
{
	m_latestPlaylistRevision++;
	m_playlistRevisions[m_latestPlaylistRevision] = playlist;
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
