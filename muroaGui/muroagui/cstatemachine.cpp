#include "cstatemachine.h"

#include <QDebug>
#include <QXmlStreamReader>


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


void CStateMachine::readCollection(QXmlStreamReader* reader)
{
	if(reader)
	{
		QXmlStreamAttributes att = reader->attributes();
	    QStringRef revision = att.value(QString(), QString("revision"));

	    bool ok;
	    m_revision = revision.toString().toULongLong(&ok);

	    qDebug() << QString("readCollection: revision %1").arg(m_revision);

	}
	else
	{
		// end tag -> readCollection is complete
	}
}

