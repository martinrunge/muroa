#include "CConnection.h"

#include <QDebug>

using namespace std;

CConnection::CConnection() {
	connect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

CConnection::~CConnection() {
    disconnect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
    disconnect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void CConnection::open(QString host, int port) {
    m_socket.connectToHost(host, port);
}

void CConnection::close() {
    m_socket.disconnectFromHost();
}

void CConnection::onDataToSend(const char *data, int length) {
<<<<<<< HEAD
	m_socket.write(data, length);
}

void CConnection::onListSessions(vector<string> sessions) {

=======
>>>>>>> 485bf2644495e28e4bf913a061d7967e02fa0fc0
}

void CConnection::onJoinSession(string sessionName) {
}

void CConnection::onLeaveSession() {
}

void CConnection::onPlay() {
}

void CConnection::onPause() {
}

void CConnection::onStop() {
}
<<<<<<< HEAD

void CConnection::onNext()
{
}

void CConnection::onPrev()
{
}

void CConnection::onStateChanged(int newState)
{
}

void CConnection::onProgress(uint32_t jobID, int progress)
{
}

=======

void CConnection::onNext()
{
}

void CConnection::onPrev()
{
}

void CConnection::onStateChanged(int newState)
{
}

void CConnection::onProgress(uint32_t jobID, int progress)
{
}

>>>>>>> 485bf2644495e28e4bf913a061d7967e02fa0fc0
void CConnection::onError(uint32_t jobID, int errorCode, std::string description)
{
}

void CConnection::onGetCollection(unsigned  knownRev)
{
}

void CConnection::onGetPlaylist(unsigned  knownRev)
{
}

void CConnection::onGetNextlist(unsigned  knownRev)
{
}

void CConnection::onCollection(unsigned  diffFromRev, std::string collection)
<<<<<<< HEAD
{
}

void CConnection::onPlaylist(unsigned  diffFromRev, std::string playlist)
{
}

void CConnection::onNextlist(unsigned  diffFromRev, std::string nextlist)
{
}

void CConnection::onEditCollection(unsigned  fromRev, std::string collectionDiff)
{
}

=======
{
}

void CConnection::onPlaylist(unsigned  diffFromRev, std::string playlist)
{
}

void CConnection::onNextlist(unsigned  diffFromRev, std::string nextlist)
{
}

void CConnection::onEditCollection(unsigned  fromRev, std::string collectionDiff)
{
}

>>>>>>> 485bf2644495e28e4bf913a061d7967e02fa0fc0
void CConnection::onEditPlaylist(unsigned  fromRev, std::string playlistDiff)
{
}

void CConnection::onEditNextlist(unsigned  fromRev, std::string nextlistDiff)
{
}

void CConnection::connected() {
<<<<<<< HEAD
    emit connectionStatusChanged( e_connected );

    QString lastSession;
    // if rejoin last session
    if( m_settings.value("rejoin", false).toBool() ) {
    	QString lastSession = m_settings.value("sesionName", "").toString();
    }

	if( !lastSession.isEmpty() ) {
		joinSession(string(lastSession.toUtf8()));
	}
    else {
    	listSessions();
    }
}

void CConnection::disconnected() {
=======
    qDebug() << QString("CStateMachine::connected");
    if(rejoinLastSession) {
    	joinSession(lastSession);
    } else {

    }
    joinSession(1);
    emit connectionStatusChanged( e_connected );
}

void CConnection::disconnected() {
    qDebug() << QString("CStateMachine::disconnected");
>>>>>>> 485bf2644495e28e4bf913a061d7967e02fa0fc0
    emit connectionStatusChanged( e_disconnected );
}

void CConnection::error() {

}

void CConnection::readyRead() {
    int avail = m_socket.bytesAvailable();
    QByteArray data = m_socket.readAll();
    newData(data.data(), data.length());
}

void CConnection::sendCommand(muroa::CCmdBase* cmd) {
    qDebug() << QString("sendCommand %1").arg(QString::fromUtf8(cmd->name().c_str()));
    delete cmd;
}

