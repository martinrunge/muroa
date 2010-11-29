#include "CStateMachine.h"
#include "CConnection.h"
#include "CSession.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlAttributes>

CStateMachine::CStateMachine(CConnection* connection) : m_connection(connection)
{
	m_xml_depth = 0;
    m_xml_reader = new QXmlStreamReader();
    m_state = e_not_connected;

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
    m_xml_depth++;
    m_state = e_connected;
    qDebug() << QString("startDocument %1 (depth %2)").arg(reader->name().toString()).arg(m_xml_depth);
}

void CStateMachine::endDocument(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    m_state = e_not_connected;
    qDebug() << QString("endDocument %1 (depth %2)").arg(reader->name().toString()).arg(m_xml_depth);
    m_xml_depth--;
}

void CStateMachine::startElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    m_xml_depth++;
    qDebug() << QString("startElement %1 (depth %2)").arg(name.toString()).arg(m_xml_depth);
    if(name.toString().startsWith("next"))
    {
        parseNextArgs(reader);
    }
    else if(name.toString().startsWith("prev"))
    {
        parsePrevArgs(reader);
    }
    else if(name.toString().startsWith("play"))
    {
        parsePlayArgs(reader);
    }
    else if(name.toString().startsWith("stop"))
    {
        parseStopArgs(reader);
    }
    else if(name.toString().startsWith("getNextlist"))
    {
        parseGetNextlistArgs(reader);
    }
    else if(name.toString().startsWith("getPlaylist"))
    {
        parseGetPlaylistArgs(reader);
    }
    else if(name.toString().startsWith("getCollection"))
    {
        parseGetCollectionArgs(reader);
    }
    else if(name.toString().startsWith("modNextlist"))
    {
        parseModNextlistArgs(reader);
    }
    else if(name.toString().startsWith("modPlaylist"))
    {
        parseModPlaylistArgs(reader);
    }
    else if(name.toString().startsWith("modCollection"))
    {
        parseModCollectionArgs(reader);
    }
    else if(name.toString().startsWith("session"))
    {
        qDebug() << QString("A client joined the session.");
    }
    else
    {
        qDebug() << QString("Unknown start tag received: %1").arg(reader->name().toString());
    }
}

void CStateMachine::endElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    if(name.toString().startsWith("next") && m_state == e_next_requested)
    {
    	m_connection->next();
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("prev") && m_state == e_prev_requested)
    {
    	m_connection->prev();
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("play") && m_state == e_play_requested)
    {
    	m_connection->play();
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("stop") && m_state == e_stop_requested)
    {
    	m_connection->stop();
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("getNextlist") && m_state == e_nextlist_requested)
    {
    	m_connection->sendNextlist(m_knownRevision);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("getNextlist") && m_state == e_nextlist_requested)
    {
    	m_connection->sendNextlist(m_knownRevision);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("getPlaylist") && m_state == e_playlist_requested)
    {
    	m_connection->sendPlaylist(m_knownRevision);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("getCollection") && m_state == e_collection_requested)
    {
    	m_connection->sendCollection(m_knownRevision);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("modNextlist") && m_state == e_awaiting_nextlist_mod)
    {
    	m_session->addNextlistRevFromDiff(&m_nextlistDiff, m_diffFromRev);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("modPlaylist") && m_state == e_awaiting_playlist_mod)
    {
    	m_session->addPlaylistRevFromDiff(&m_playlistDiff, m_diffFromRev);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("modCollection") && m_state == e_awaiting_collection_mod)
    {
    	m_session->addCollectionRevFromDiff(&m_collectionDiff, m_diffFromRev);
    	m_state = e_connected;
    }
    else if(name.toString().startsWith("session"))
    {
        qDebug() << QString("This client want's to leave");
    }
    else
    {
        qDebug() << QString("Unknown end tag received: %1").arg(reader->name().toString());
    }
    qDebug() << QString("endElement %1 (depth %2)").arg(reader->name().toString()).arg(m_xml_depth);
    m_xml_depth--;
}

void CStateMachine::characters(QXmlStreamReader* reader)
{
    switch(m_state)
    {
    case e_awaiting_nextlist_mod:
    	m_nextlistDiff = reader->text().toString();
    	qDebug() << QString("characters: %1").arg(m_nextlistDiff);
    	break;

    case e_awaiting_playlist_mod:
    	m_playlistDiff = reader->text().toString();
    	qDebug() << QString("characters: %1").arg(m_playlistDiff);
    	break;

    case e_awaiting_collection_mod:
    	m_collectionDiff = reader->text().toString();
    	break;

   	default:
   	    qDebug() << QString("receiving characters while in unknown state....");
    }
}

void CStateMachine::parseNextArgs(QXmlStreamReader* reader)
{
    if( m_state == e_connected )
    {
    	m_state = e_next_requested;
    }
}

void CStateMachine::parsePrevArgs(QXmlStreamReader* reader)
{
    if( m_state == e_connected )
    {
    	m_state = e_prev_requested;
    }
}

void CStateMachine::parseStopArgs(QXmlStreamReader* reader)
{
    if( m_state == e_connected )
    {
    	m_state = e_stop_requested;
    }
}

void CStateMachine::parsePlayArgs(QXmlStreamReader* reader)
{
    if( m_state == e_connected )
    {
    	m_state = e_play_requested;
    }
}

void CStateMachine::parseGetNextlistArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("knownRev") && m_state == e_connected )
    {
	    QStringRef knownRevision = att.value(QString(), QString("knownRev"));

	    bool ok;
	    m_knownRevision = knownRevision.toString().toULongLong(&ok);

	    qDebug() << QString("getNextlist: knownRevision %1").arg(m_knownRevision);
    }
    else
    {
    	// client does not have and old revision, send whole nextlist
    	m_knownRevision = -1;
    }
	m_state = e_nextlist_requested;
}

void CStateMachine::parseGetPlaylistArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("knownRev") && m_state == e_connected )
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
	m_state = e_playlist_requested;
}

void CStateMachine::parseGetCollectionArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("knownRev") && m_state == e_connected )
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
	m_state = e_collection_requested;
}

void CStateMachine::parseModNextlistArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("fromRev") && m_state == e_connected )
    {
	    QStringRef fromRevision = att.value(QString(), QString("fromRev"));

	    bool ok;
	    m_diffFromRev = fromRevision.toString().toUInt(&ok);
	    if(ok) {
	    	m_state = e_awaiting_nextlist_mod;
	    }

	    qDebug() << QString("parseNextlistMod: diff from Rev %1").arg(m_diffFromRev);
    }
    else
    {
    	// no fromRev included. ignore.
    }
}

void CStateMachine::parseModPlaylistArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("fromRev") && m_state == e_connected )
    {
	    QStringRef fromRevision = att.value(QString(), QString("fromRev"));

	    bool ok;
	    m_diffFromRev = fromRevision.toString().toUInt(&ok);
	    if(ok) {
	    	m_state = e_awaiting_playlist_mod;
	    }

	    qDebug() << QString("parsePlaylistMod: diff from Rev %1").arg(m_diffFromRev);
    }
    else
    {
    	// no fromRev included. ignore.
    }
}

void CStateMachine::parseModCollectionArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();
    if(att.hasAttribute("fromRev") && m_state == e_connected )
    {
	    QStringRef fromRevision = att.value(QString(), QString("fromRev"));

	    bool ok;
	    m_diffFromRev = fromRevision.toString().toUInt(&ok);
	    if(ok) {
	    	m_state = e_awaiting_collection_mod;
	    }
	    qDebug() << QString("parsePlaylistMod: diff from Rev %1").arg(m_diffFromRev);
    }
    else
    {
    	// no fromRev included. ignore.
    }
}

