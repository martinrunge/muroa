#include "cstatemachine.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlAttributes>

CStateMachine::CStateMachine()
{
	m_revision = 0;
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
    if(name.toString().startsWith("getCollection"))
    {
        parseWriteArgs(reader);
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
    qDebug() << QString("startElement %1").arg(reader->name().toString());
}

void CStateMachine::endElement(QXmlStreamReader* reader)
{
    QStringRef name = reader->name();
    if(name.toString().startsWith("getCollection"))
    {
        sendCollection();
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



void CStateMachine::parseReadArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();

}


void CStateMachine::parseWriteArgs(QXmlStreamReader* reader)
{
    QXmlStreamAttributes att = reader->attributes();

}



void CStateMachine::sendCollection()
{
    m_xml_writer->writeStartElement("collection");
    m_xml_writer->writeAttribute("revision", QString().setNum(m_revision));
    m_xml_writer->writeEndElement();
}

