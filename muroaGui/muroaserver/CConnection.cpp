
#include "CConnection.h"

#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QTcpSocket>
#include <QDebug>

CConnection::CConnection(QTcpSocket* socket) : m_socket(socket), m_sm(this)
{
    // CNetwork *m_net = new CNetwork(2678);

    connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));


    //connect(m_net,SIGNAL(newConnection(QIODevice*)), this, SLOT(newConnection(QIODevice*)));
    //connect(m_net,SIGNAL(connectionClosed(QIODevice*)), this, SLOT(connectionClosed(QIODevice*)));

    qDebug() << QString("CConnection::newConnection");
    // m_io_dev = ioDev;
    m_xml_writer = new QXmlStreamWriter(m_socket);
    m_xml_writer->setAutoFormatting(true);
    m_xml_writer->writeStartDocument(QString("1.0"), true);
    m_xml_writer->writeStartElement("session");

    m_sm.setXmlWriter(m_xml_writer);

    connect(m_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    emit connectionStatusChanged(QString("connected ..."));

}


CConnection::~CConnection()
{
    // delete m_net;
}


void CConnection::readyRead()
{
    int avail = m_socket->bytesAvailable();
    qDebug() << QString("readyRead(): %1 bytes available").arg(avail);
    QByteArray data = m_socket->readAll();
    qDebug() << QString("readyRead(): %1 bytes read").arg(data.size());
    m_sm.addData(data);
}


void CConnection::disconnected()
{
    qDebug() << QString("socket disconnected");
    disconnect(m_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    // delete m_xml_reader;

    emit connectionStatusChanged(QString("disconnected"));


    disconnect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
    disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    emit connectionClosed(this);

    m_socket = 0;
}

void CConnection::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << QString("socket error %1").arg(m_socket->errorString());

}

void CConnection::connected()
{
    qDebug() << QString("socket connected");

}

void CConnection::next()
{
    m_session->next();
}

void CConnection::prev()
{
    m_session->prev();
}

void CConnection::play()
{
    m_session->play();
}

void CConnection::stop()
{
    m_session->stop();
}

void CConnection::sendCollection(int knownRevision)
{
	QString collection;

    m_xml_writer->writeStartElement("collection");
    m_xml_writer->writeAttribute("revision", QString().setNum(m_session->getCollectionRevision()));

    collection = m_session->getCollectionDiff(knownRevision);
    if(!collection.isNull())
    {
    	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(knownRevision));
    }
    else
    {
    	collection = m_session->getCollection()->getText();
    }
    // qDebug() << collection;
    m_xml_writer->writeCharacters(collection);
    m_xml_writer->writeEndElement();
}

void CConnection::sendPlaylist(int knownRevision)
{
	QString playlist;

    m_xml_writer->writeStartElement("playlist");
    m_xml_writer->writeAttribute("revision", QString().setNum(m_session->getPlaylistRevision()));

    playlist = m_session->getPlaylistDiff(knownRevision);
    if(!playlist.isNull())
    {
    	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(knownRevision));
    }
    else
    {
    	playlist = m_session->getPlaylist()->getText();
    }
    // qDebug() << collection;
    m_xml_writer->writeCharacters(playlist);
    m_xml_writer->writeEndElement();
}

void CConnection::sendNextlist(int knownRevision)
{
	QString nextlist;

    m_xml_writer->writeStartElement("nextlist");
    m_xml_writer->writeAttribute("revision", QString().setNum(m_session->getNextlistRevision()));

    nextlist = m_session->getNextlistDiff(knownRevision);
    if(!nextlist.isNull())
    {
    	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(knownRevision));
    }
    else
    {
    	nextlist = m_session->getNextlist()->getText();
    }

    m_xml_writer->writeCharacters(nextlist);
    m_xml_writer->writeEndElement();
}


void CConnection::sendProgress(int done, int total)
{
    m_xml_writer->writeStartElement("progress");
    m_xml_writer->writeAttribute("done", QString().setNum(done));
    m_xml_writer->writeAttribute("total", QString().setNum(total));
    m_xml_writer->writeEndElement();
}
