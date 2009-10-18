#include "cstatemachine.h"

#include <QDebug>
#include <QXmlStreamReader>


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
    if(name.toString().startsWith("collection"))
    {
    	readCollection(reader);
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
    if(name.toString().startsWith("collection"))
    {
        readCollection();
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
		case e_reading_collection:
			parseCollection(text);
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


void CStateMachine::readCollection(QXmlStreamReader* reader)
{
	if(reader)
	{
		QXmlStreamAttributes att = reader->attributes();
	    QStringRef revision = att.value(QString(), QString("revision"));

	    bool ok;
	    m_revision = revision.toString().toULongLong(&ok);

	    qDebug() << QString("readCollection: revision %1").arg(m_revision);

	    m_state = e_reading_collection;

	}
	else
	{
		// end tag -> readCollection is complete
	    m_state = e_collection_received;
	}
}


void CStateMachine::parseCollection(QStringRef text)
{
	QString characters = text.toString();
	QTextStream stream(&characters, QIODevice::ReadOnly);
	QString line;
	QList<CCollectionItem> items;

	qDebug() << text;

	do {
	    line = stream.readLine();
	    CCollectionItem newItem;
	    // line is supposed to be comma seperated

    	bool ok;
    	// newItem.setFilename( line.section(',', 0, 0) );
    	newItem.setArtist( line.section(',', 0, 0) );
    	newItem.setAlbum( line.section(',', 1, 1) );
    	newItem.setYear( line.section(',', 2, 2).toInt(&ok) );
    	newItem.setTitle( line.section(',', 3, 3) );
    	newItem.setLengthInSec( line.section(',', 4, 4).toInt(&ok) );

	    items.append(newItem);


	} while (!line.isNull());
	m_collectionModelPtr->append(items);

}
