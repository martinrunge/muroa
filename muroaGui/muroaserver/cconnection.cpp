
#include "cconnection.h"

#include <QXmlSimpleReader>
#include <QXmlInputSource>

CConnection::CConnection()
{
    CNetwork *m_net = new CNetwork(2678);

    connect(m_net,SIGNAL(newConnection(QIODevice*)), this, SLOT(newConnection(QIODevice*)));
    connect(m_net,SIGNAL(connectionClosed(QIODevice*)), this, SLOT(connectionClosed(QIODevice*)));

}


CConnection::~CConnection()
{
    delete m_net;
}


void CConnection::newConnection(QIODevice* ioDev)
{
    qDebug() << QString("CConnection::newConnection");
    m_io_dev = ioDev;
    m_xml_reader = new QXmlStreamReader();
    m_xml_writer = new QXmlStreamWriter(ioDev);
    m_xml_writer->setAutoFormatting(true);
    m_xml_writer->writeStartDocument(QString("1.0"), true);
    m_xml_writer->writeStartElement("muroa_server");

    m_sm.setXmlWriter(m_xml_writer);

    connect(m_io_dev, SIGNAL(readyRead()),this, SLOT(readyRead()));

    emit connectionStatusChanged(QString("connected ..."));
}

void CConnection::connectionClosed(QIODevice* ioDev)
{
    qDebug() << QString("CConnection::connectionClosed");
    disconnect(m_io_dev, SIGNAL(readyRead()),this, SLOT(readyRead()));
//    m_xml_reader->parse(m_xml_src, true);
    delete m_xml_reader;

    emit connectionStatusChanged(QString("disconnected"));

}


void CConnection::readyRead()
{
    int avail = m_io_dev->bytesAvailable();
    qDebug() << QString("readyRead(): %1 bytes available").arg(avail);
    QByteArray data = m_io_dev->readAll();
    qDebug() << QString("readyRead(): %1 bytes read").arg(data.size());
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
                m_sm.startDocument(m_xml_reader);
                break;
            case QXmlStreamReader::EndDocument:
                m_sm.endDocument(m_xml_reader);
                break;
            case QXmlStreamReader::StartElement:
                m_sm.startElement(m_xml_reader);
                break;
            case QXmlStreamReader::EndElement:
                m_sm.endElement(m_xml_reader);
                break;
            case QXmlStreamReader::Characters:
                m_sm.characters(m_xml_reader);
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
