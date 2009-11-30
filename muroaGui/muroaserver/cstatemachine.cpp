#include "cstatemachine.h"
#include "cconnection.h"
#include "CSession.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlAttributes>

CStateMachine::CStateMachine(CConnection* connection) : m_connection(connection)
{
	//m_revision = 0;
    m_xml_reader = new QXmlStreamReader();

}

CStateMachine::~CStateMachine()
{
	delete m_xml_reader;
}

void CStateMachine::close()
{

}

void CStateMachine::open()
{

}

void CStateMachine::addData(QByteArray data)
{
    m_xml_reader->addData(data);
    enum QXmlStreamReader::TokenType tokenType;
    do
    {
        tokenType = m_xml_reader->readNext();
        switch(tokenType)
        {
            case QXmlStreamReader::NoToken:
                // nothing yet
                break;
            case QXmlStreamReader::Invalid:
                // check if we have to wait for new data
                if( m_xml_reader->error() != QXmlStreamReader::PrematureEndOfDocumentError )
                {
                    //there is a real error
                    qDebug() << QString("Parse Error: %1   line %2 col %3  offset %4").arg(m_xml_reader->errorString()).arg(m_xml_reader->lineNumber()).arg(m_xml_reader->columnNumber()).arg(m_xml_reader->characterOffset());
                    return;
                }
                break;
            case QXmlStreamReader::StartDocument:
                startDocument(m_xml_reader);
                break;
            case QXmlStreamReader::EndDocument:
                endDocument(m_xml_reader);
                break;
            case QXmlStreamReader::StartElement:
                startElement(m_xml_reader);
                break;
            case QXmlStreamReader::EndElement:
                endElement(m_xml_reader);
                break;
            case QXmlStreamReader::Characters:
                characters(m_xml_reader);
                break;
            case QXmlStreamReader::Comment:
                // nothing yet
                break;
            case QXmlStreamReader::DTD:
                // nothing yet
                break;
            case QXmlStreamReader::EntityReference:
                // nothing yet
                break;
            case QXmlStreamReader::ProcessingInstruction:
                // nothing yet
                break;

            default:
                // nothing yet
                break;
        }
    } while(tokenType != QXmlStreamReader::Invalid );
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
    qDebug() << QString("startElement %1").arg(name.toString());
    if(name.toString().startsWith("getPlaylist"))
    {
        parseGetPlaylistArgs(reader);
    }
    else if(name.toString().startsWith("getCollection"))
    {
        parseGetCollectionArgs(reader);
    }
    else if(name.toString().startsWith("addSong"))
    {
        parseReadArgs(reader);
    }
    else if(name.toString().startsWith("muroa_session"))
    {
        qDebug() << QString("Begin of Document");
    }
    else
    {
        qDebug() << QString("Unknown tag received: %1").arg(reader->name().toString());
    }
}

void CStateMachine::endElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    if(name.toString().startsWith("getPlaylist"))
    {
    	m_connection->sendPlaylist(m_knownRevision);
    }
    else if(name.toString().startsWith("getCollection"))
    {
    	m_connection->sendCollection(m_knownRevision);
    }
    else if(name.toString().startsWith("addSong"))
    {
        parseReadArgs(reader);
    }
    else if(name.toString().startsWith("muroa_session"))
    {
        qDebug() << QString("Begin of Document");
    }
    else
    {
        qDebug() << QString("Unknown tag received: %1").arg(reader->name().toString());
    }
    qDebug() << QString("endElement %1").arg(reader->name().toString());
}

void CStateMachine::characters(QXmlStreamReader* reader)
{
    qDebug() << QString("characters");
}

void CStateMachine::parseGetPlaylistArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("knownRev"))
    {
	    QStringRef knownRevision = att.value(QString(), QString("knownRev"));

	    bool ok;
	    m_knownRevision = knownRevision.toString().toULongLong(&ok);

	    qDebug() << QString("getPlaylist: knownRevision %1").arg(m_knownRevision);
    }
    else
    {
    	// client does not have and old revision, send whole collection
    	m_knownRevision = -1;
    }
}

void CStateMachine::parseGetCollectionArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("knownRev"))
    {
	    QStringRef knownRevision = att.value(QString(), QString("knownRev"));

	    bool ok;
	    m_knownRevision = knownRevision.toString().toULongLong(&ok);

	    qDebug() << QString("getCollection: knownRevision %1").arg(m_knownRevision);
    }
    else
    {
    	// client does not have and old revision, send whole collection
    	m_knownRevision = -1;
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


//
//void CStateMachine::sendCollection()
//{
//	QString collection;
//
//    m_xml_writer->writeStartElement("collection");
//    m_xml_writer->writeAttribute("revision", QString().setNum(m_session->getCollectionRevision()));
//
//    collection = m_session->getCollectionDiff(m_knownRevision);
//    if(!collection.isNull())
//    {
//    	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(m_knownRevision));
//    }
//    else
//    {
//    	collection = m_session->getCollection();
//    }
//    qDebug() << collection;
//    m_xml_writer->writeCharacters(collection);
//    m_xml_writer->writeEndElement();
//}

