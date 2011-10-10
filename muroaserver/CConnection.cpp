
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

void CConnection::sendCollection() {
	CCollection<CCollectionItem>* collection = m_session->getCollection();
	sendCollection(collection);
}

void CConnection::sendCollection(CCollection<CCollectionItem>* collection)
{
    m_xml_writer->writeStartElement("collection");
    m_xml_writer->writeAttribute("revision", QString().setNum(collection->getRevision()));
    m_xml_writer->writeCharacters(collection->getText());
    m_xml_writer->writeEndElement();
}

void CConnection::sendCollectionDiff(int diffFromRev) {
	if(diffFromRev > m_session->getCollectionRevision()) {
		sendCollection(m_session->getCollection());
	}
	else {
		QString collectionDiff = m_session->getCollectionDiff(diffFromRev);
		sendCollectionDiff(diffFromRev, m_session->getCollectionRevision(), collectionDiff);
	}
}


void CConnection::sendCollectionDiff(int revision, int diffFromRev, QString diff) {
	m_xml_writer->writeStartElement("collection");
	m_xml_writer->writeAttribute("revision", QString().setNum(revision));
	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(diffFromRev));
    m_xml_writer->writeCharacters(diff);
    m_xml_writer->writeEndElement();
}

void CConnection::sendPlaylist() {
	CCollection<CPlaylistItem>* playlist = m_session->getPlaylist();
	sendPlaylist(playlist);
}

void CConnection::sendPlaylist(CCollection<CPlaylistItem>* playlist)
{
    m_xml_writer->writeStartElement("playlist");
    m_xml_writer->writeAttribute("revision", QString().setNum(playlist->getRevision()));
    m_xml_writer->writeCharacters(playlist->getText());
    m_xml_writer->writeEndElement();
}

void CConnection::sendPlaylistDiff(int diffFromRev) {
	if(diffFromRev > m_session->getPlaylistRevision()) {
		sendPlaylist(m_session->getPlaylist());
	}
	else {
		QString playlistDiff = m_session->getPlaylistDiff(diffFromRev);
		sendPlaylistDiff(diffFromRev, m_session->getPlaylistRevision(), playlistDiff);
	}
}

void CConnection::sendPlaylistDiff(int revision, int diffFromRev, QString diff) {

    m_xml_writer->writeStartElement("playlist");
    m_xml_writer->writeAttribute("revision", QString().setNum(revision));
   	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(diffFromRev));
    m_xml_writer->writeCharacters(diff);
    m_xml_writer->writeEndElement();
}

void CConnection::sendNextlist() {
	CCollection<CPlaylistItem>* nextlist = m_session->getNextlist();
	sendNextlist(nextlist);
}

void CConnection::sendNextlist(CCollection<CPlaylistItem>* nextlist)
{
    m_xml_writer->writeStartElement("nextlist");
    m_xml_writer->writeAttribute("revision", QString().setNum(nextlist->getRevision()));
    m_xml_writer->writeCharacters(nextlist->getText());
    m_xml_writer->writeEndElement();
}

void CConnection::sendNextlistDiff(int diffFromRev) {
	if(diffFromRev > m_session->getNextlistRevision()) {
		sendNextlist(m_session->getNextlist());
	}
	else {
		QString nextlistDiff = m_session->getNextlistDiff(diffFromRev);
		sendNextlistDiff(diffFromRev, m_session->getNextlistRevision(), nextlistDiff);
	}
}

void CConnection::sendNextlistDiff(int revision, int diffFromRev, QString diff) {

	m_xml_writer->writeStartElement("nextlist");
    m_xml_writer->writeAttribute("revision", QString().setNum(revision));
	m_xml_writer->writeAttribute("diffFromRev", QString().setNum(diffFromRev));
    m_xml_writer->writeCharacters(diff);
    m_xml_writer->writeEndElement();
}

int CConnection::addCollectionRevFromDiff(QString* collectionDiff, int diffFromRev) {
	try {
		m_session->addCollectionRevFromDiff(collectionDiff, diffFromRev);
	}
	catch(InvalidMsgException invalEx) {
		reportError(invalEx.what());
	}
}

int CConnection::addPlaylistRevFromDiff(QString* playlistDiff, int diffFromRev) {
	m_session->addPlaylistRevFromDiff(playlistDiff, diffFromRev);
}

int CConnection::addNextlistRevFromDiff(QString* nextlistDiff, int diffFromRev) {
	m_session->addNextlistRevFromDiff(nextlistDiff, diffFromRev);
}



void CConnection::sendProgress(int done, int total)
{
    m_xml_writer->writeStartElement("progress");
    m_xml_writer->writeAttribute("done", QString().setNum(done));
    m_xml_writer->writeAttribute("total", QString().setNum(total));
    m_xml_writer->writeEndElement();
}


void CConnection::reportError(QString error) {
    m_xml_writer->writeStartElement("error");
    m_xml_writer->writeCharacters(error);
    m_xml_writer->writeEndElement();
}
