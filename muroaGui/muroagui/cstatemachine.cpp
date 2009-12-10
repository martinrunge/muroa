#include "cstatemachine.h"

#include <typeinfo>

#include <QDebug>
#include <QXmlStreamReader>
#include <QRegExp>

#include "CCollection.h"
#include "CCollectionItem.h"
#include "CPlaylistItem.h"

#include "CPlaylistModel.h"
#include "CCollectionModel.h"

CStateMachine::CStateMachine()
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

    qDebug() << QString("startDocument %1").arg(reader->name().toString());
}

void CStateMachine::endDocument(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    qDebug() << QString("endDocument %1").arg(reader->name().toString());
}


void CStateMachine::startElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    m_xml_depth++;
    if(name.toString().startsWith("playlist"))
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
    else if(name.toString().startsWith("muroa_server"))
    {
        qDebug() << QString("Begin of Document");
        m_state = e_session_active;
    }
    else
    {
        qDebug() << QString("Unknown tag received: %1").arg(reader->name().toString());
    }
    qDebug() << QString("startElement %1").arg(reader->name().toString());
}

void CStateMachine::endElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    m_xml_depth--;
    if(name.toString().startsWith("playlist"))
    {
	    m_state = e_playlist_received;
    }
    else if(name.toString().startsWith("collection"))
    {
	    m_state = e_collection_received;
    }
    else if(name.toString().startsWith("addSong"))
    {
        parseReadArgs(reader);
    }
    else if(name.toString().startsWith("muroa_server"))
    {
        qDebug() << QString("Begin of Document");
        m_state = e_no_session;
    }
    else
    {
        qDebug() << QString("Unknown tag received: %1").arg(reader->name().toString());
    }
    qDebug() << QString("endElement %1").arg(reader->name().toString());
}

void CStateMachine::characters(QXmlStreamReader* reader)
{
    // qDebug() << QString("characters");
	QStringRef text = reader->text();
	switch(m_state)
	{
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
				qDebug() << QString("Received unexpected characters: ").append(text);
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

void CStateMachine::parsePlaylistArgs(QXmlStreamReader* reader)
{
	QXmlStreamAttributes att = reader->attributes();
    QStringRef revision = att.value(QString(), QString("revision"));

    m_diffFromRev = -1;
    bool ok;
    m_revision = revision.string()->toULongLong(&ok);

    if(att.hasAttribute("diffFromRev"))
    {
	    QStringRef diffFromRev = att.value(QString(), QString("diffFromRev"));
	    bool ok;
	    m_diffFromRev = diffFromRev.string()->toULongLong(&ok);
    }

    qDebug() << QString("readOlaylist: revision %1").arg(m_revision);

    m_state = e_reading_playlist;

}


void CStateMachine::parseCollectionArgs(QXmlStreamReader* reader)
{
	QXmlStreamAttributes att = reader->attributes();
    QStringRef revision = att.value(QString(), QString("revision"));

    m_diffFromRev = -1;
    bool ok;
    m_revision = revision.string()->toULongLong(&ok);

    if(att.hasAttribute("diffFromRev"))
    {
	    QStringRef diffFromRev = att.value(QString(), QString("diffFromRev"));
	    bool ok;
	    m_diffFromRev = diffFromRev.string()->toULongLong(&ok);
    }

    qDebug() << QString("readCollection: revision %1").arg(m_revision);

    m_state = e_reading_collection;

}


template <typename T> void CStateMachine::parseCollection(QStringRef text, CModelBase<T>* model)
{
	QString characters = text.toString();
	QTextStream stream(&characters, QIODevice::ReadOnly);
	QString line;
	QList<T> items;
	qDebug() << text;

	do {
	    line = stream.readLine();
	    if(line.isEmpty()) continue;
	    T newItem(line);
	    items.append(newItem);
	} while (!line.isNull());

	model->append(items);
}

template <typename T> void CStateMachine::parseCollectionDiff(QStringRef text, CModelBase<T>* model)
{
	QString characters = text.toString();
	QTextStream stream(&characters, QIODevice::ReadOnly);
	QString line;
	QList<T> items;

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
					T newItem(content);
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
