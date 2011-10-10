#include "CStateMachine.h"

#include <typeinfo>

#include <QDebug>
#include <QXmlStreamReader>
#include <QRegExp>

#include "CConnection.h"

#include "CCollection.h"
#include "CCollectionItem.h"
#include "CPlaylistItem.h"

#include "CPlaylistModel.h"
#include "CCollectionModel.h"

CStateMachine::CStateMachine(CConnection* conn) : m_connectionPtr(conn)
{
	m_revision = 0;
	m_state = e_no_session;
}

CStateMachine::~CStateMachine()
{

}

void CStateMachine::close()
{

}

void CStateMachine::open()
{

}

void CStateMachine::startDocument(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    //qDebug() << QString("startDocument %1").arg(reader->name().toString());
}

void CStateMachine::endDocument(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
//    qDebug() << QString("endDocument %1").arg(reader->name().toString());
}


void CStateMachine::startElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    m_xml_depth++;
    if(name.toString().startsWith("progress"))
    {
    	parseProgressArgs(reader);
    }
    else if(name.toString().startsWith("stateChanged"))
    {
    	parseStateChangedArgs(reader);
    }
    else if(name.toString().startsWith("nextlist"))
    {
    	parseNextlistArgs(reader);
    }
    else if(name.toString().startsWith("playlist"))
    {
    	parsePlaylistArgs(reader);
    }
    else if(name.toString().startsWith("collection"))
    {
    	parseCollectionArgs(reader);
    }
    else if(name.toString().startsWith("addSong"))
    {
        parseReadArgs(reader);
    }
    else if(name.toString().startsWith("session"))
    {
//        qDebug() << QString("Begin of Document");
        m_state = e_session_active;
    }
    else
    {
        qDebug() << QString("Unknown start tag received: %1").arg(reader->name().toString());
    }
//    qDebug() << QString("startElement %1").arg(reader->name().toString());
}

void CStateMachine::endElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    m_xml_depth--;
    if(name.toString().startsWith("progress") && m_state == e_progress)
    {
    	emit progress(m_done, m_total);
	    m_state = e_session_active;
    }
    else if(name.toString().startsWith("stateChanged") && m_state == e_state_changed)
    {
    	emit stateChanged(m_sessionState);
	    m_state = e_session_active;
    }
    else if(name.toString().startsWith("nextlist"))
    {
    	m_nextlistModelPtr->setRevision(m_revision);
	    m_state = e_nextlist_received;
    }
    else if(name.toString().startsWith("playlist"))
    {
    	m_playlistModelPtr->setRevision(m_revision);
	    m_state = e_playlist_received;
    }
    else if(name.toString().startsWith("collection"))
    {
    	m_collectionModelPtr->setRevision(m_revision);
	    m_state = e_collection_received;
    }
    else if(name.toString().startsWith("addSong"))
    {
        parseReadArgs(reader);
    }
    else if(name.toString().startsWith("session"))
    {
        // qDebug() << QString("End of Document");
        m_state = e_no_session;
    }
    else
    {
        qDebug() << QString("Unknown end tag received: %1").arg(reader->name().toString());
    }
    // qDebug() << QString("endElement %1").arg(reader->name().toString());
}

void CStateMachine::characters(QXmlStreamReader* reader)
{
    // qDebug() << QString("characters");
	QStringRef text = reader->text();
	switch(m_state)
	{
		case e_reading_nextlist:
			if(m_diffFromRev == -1)
			{
				parseCollection<CPlaylistItem>(text, m_nextlistModelPtr);
			}
			else
			{
				parseCollectionDiff<CPlaylistItem>(text, m_nextlistModelPtr);
			}
			break;

		case e_reading_playlist:
			if(m_diffFromRev == -1)
			{
				parseCollection<CPlaylistItem>(text, m_playlistModelPtr);
			}
			else
			{
				parseCollectionDiff<CPlaylistItem>(text, m_playlistModelPtr);
			}
			break;

		case e_reading_collection:
			if(m_diffFromRev == -1)
			{
				parseCollection<CCollectionItem>(text, m_collectionModelPtr);
			}
			else
			{
				parseCollectionDiff<CCollectionItem>(text, m_collectionModelPtr);
			}
			break;

		default:
			if(text.size() > 0)
			{
				if( ! text.toString().trimmed().isEmpty() ) {
					// not interested in whitespaces from indentation here.
					qDebug() << QString("Received unexpected characters: ").append(text);
				}
			}
	}
}



void CStateMachine::parseReadArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();

}


void CStateMachine::parseWriteArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();


}

void CStateMachine::parseProgressArgs(QXmlStreamReader* reader)
{
	m_state = e_progress;

	QXmlStreamAttributes att = reader->attributes();
    QStringRef doneStrRef = att.value(QString(), QString("done"));
    QStringRef totalStrRef = att.value(QString(), QString("total"));

    bool ok;
    m_done = doneStrRef.toString().toInt(&ok);
    if(!ok) m_done = -1;
    m_total = totalStrRef.toString().toInt(&ok);
    if(!ok) m_total = -1;
}

void CStateMachine::parseStateChangedArgs(QXmlStreamReader* reader)
{
	m_state = e_state_changed;

	QXmlStreamAttributes att = reader->attributes();
    QStringRef stateStrRef = att.value(QString(), QString("state"));

    bool ok;
    m_sessionState = stateStrRef.toString().toInt(&ok);
    if(!ok) m_sessionState = e_stopped;
}



void CStateMachine::parseNextlistArgs(QXmlStreamReader* reader)
{
	QXmlStreamAttributes att = reader->attributes();
    QStringRef revision = att.value(QString(), QString("revision"));

    m_diffFromRev = -1;
    bool ok;
    m_revision = revision.toString().toULongLong(&ok);

    if(att.hasAttribute("diffFromRev"))
    {
	    QStringRef diffFromRev = att.value(QString(), QString("diffFromRev"));
	    bool ok;
	    m_diffFromRev = diffFromRev.toString().toULongLong(&ok);
    }

    qDebug() << QString("readNextlist: revision %1").arg(m_revision);

    m_state = e_reading_nextlist;
}


void CStateMachine::parsePlaylistArgs(QXmlStreamReader* reader)
{
	QXmlStreamAttributes att = reader->attributes();
    QStringRef revision = att.value(QString(), QString("revision"));

    m_diffFromRev = -1;
    bool ok;
    m_revision = revision.toString().toULongLong(&ok);

    if(att.hasAttribute("diffFromRev"))
    {
	    QStringRef diffFromRev = att.value(QString(), QString("diffFromRev"));
	    bool ok;
	    m_diffFromRev = diffFromRev.toString().toULongLong(&ok);
    }

    qDebug() << QString("readPlaylist: revision %1").arg(m_revision);

    m_state = e_reading_playlist;

}


void CStateMachine::parseCollectionArgs(QXmlStreamReader* reader)
{
	QXmlStreamAttributes att = reader->attributes();
    QStringRef revision = att.value(QString("revision"));

    m_diffFromRev = -1;
    bool ok;
    QString refStr = revision.toString();
    qDebug() << QString("revision=%1").arg(refStr);
    m_revision = refStr.toInt(&ok);

    if(att.hasAttribute("diffFromRev"))
    {
	    QStringRef diffFromRev = att.value(QString("diffFromRev"));
	    bool ok;
	    m_diffFromRev = diffFromRev.toString().toULongLong(&ok);
    }

    qDebug() << QString("readCollection: revision %1").arg(m_revision);

    m_state = e_reading_collection;

}


template <typename T> void CStateMachine::parseCollection(QStringRef text, CModelBase<T*>* model)
{
	QString characters = text.toString();
	QTextStream stream(&characters, QIODevice::ReadOnly);
	QString line;
	QList<T*> items;
	qDebug() << text;

	do {
	    line = stream.readLine();
	    if(line.isEmpty()) continue;
	    T* newItem = new T(line);
	    items.append(newItem);
	} while (!line.isNull());
	int count = model->rowCount(QModelIndex());
	model->removeItems(0, count);

	model->append(items);
}

template <typename T> void CStateMachine::parseCollectionDiff(QStringRef text, CModelBase<T*>* model)
{
	QString characters = text.toString();
	QTextStream stream(&characters, QIODevice::ReadOnly);
	QString line;
	QList<T*> items;

	qDebug() << text;

	QRegExp rxdiff("^@@ -(\\d+),(\\d+)\\s+\\+(\\d+),(\\d+)\\s*@@$");

	int oldStart(0);
	int oldLen(0);
	int newStart(0);
	int newLen(0);

	int lineNr(0);
	int chunkSizeSum(0);

	do {
	    line = stream.readLine();
	    if(line.isEmpty()) continue;
	    if(line.startsWith("@@")) // diff chunk header
	    {
	    	int pos = rxdiff.indexIn(line);
	    	if (pos > -1) {
	    		QString oldStartStr = rxdiff.cap(1);
	    		QString oldLenStr = rxdiff.cap(2);
	    		QString newStartStr = rxdiff.cap(3);
	    		QString newLenStr = rxdiff.cap(4);
	    		bool ok;
	    		oldStart = oldStartStr.toInt(&ok);
	    		oldLen = oldLenStr.toInt(&ok);
	    		newStart = newStartStr.toInt(&ok);
	    		newLen = newLenStr.toInt(&ok);

	    		if(oldLen == 0) oldStart++;
	    		lineNr = oldStart + chunkSizeSum;

	    		chunkSizeSum += newLen - oldLen;
	    		// qDebug() << QString("- %1,%2 + %3,%4").arg(oldStart).arg(oldLen).arg(newStart).arg(newLen);
	    	}
	    }
	    else
	    {
	    	QChar sign = line.at(0);
	    	QString content = line.right(line.size() - 1);

	    	switch(sign.unicode()){
				case '+': //insert
				{
					//qDebug() << QString("adding line : %1").arg(lineNr);
					//qDebug() << QString("from diff : %1").arg(content);
					T* newItem = new T(content);
					model->insertItem( newItem, lineNr - 1);
					//qDebug() << QString("collection: %1").arg(m_collectionModelPtr->getItemAsString(lineNr - 1));
					break;
				}
				case '-': //remove
					if(content.compare(model->getItemAsString(lineNr - 1)) != 0 )	// possible error:
					{
						qDebug() << QString("Error when removing line %1:").arg(lineNr);
						qDebug() << QString("line expected from diff : %1").arg(content);
						qDebug() << QString("differs form collection : %1").arg(model->getItemAsString(lineNr - 1));
					}
					model->removeItem(lineNr - 1);
					lineNr--;
					break;
				case ' ': //check
					if(content.compare(model->getItemAsString(lineNr - 1)) != 0 )	// possible error:
					{
						qDebug() << QString("Error when keeping line %1:").arg(lineNr);
						qDebug() << QString("line expected from diff : %1").arg(content);
						qDebug() << QString("differs from collection : %1").arg(model->getItemAsString(lineNr - 1));
					}
					break;
				default:
					break;

	    	}
			lineNr++;
	    }
	    // check if line stays equal, is added or removed

	} while (!line.isNull());
	// m_collectionModelPtr->append(items);
}
