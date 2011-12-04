#include "CConnection.h"
#include "cmds/CmdBase.h"
#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"
#include "muroaConstants.h"

#include <QDebug>

using namespace std;

CConnection::CConnection() : m_sm(this) {
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
	m_socket.write(data, length);
}

void CConnection::onListSessions(vector<string> sessions) {
	if(sessions.size() >= 1) {
		string sname = sessions[0];
		if(sname.compare(CREATE_NEW_SESSION) == 0) {
			sname = "default";
		}
		doJoinSession(sname);
	}
	else {
		// present a list of all session to user
	}
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

void CConnection::onEditPlaylist(unsigned  fromRev, std::string playlistDiff)
{
}

void CConnection::onEditNextlist(unsigned  fromRev, std::string nextlistDiff)
{
}

void CConnection::play()
{
	CMuroaXml::play();
}

void CConnection::stop()
{
	CMuroaXml::stop();
}

void CConnection::next() {
	CMuroaXml::next();
}

void CConnection::prev() {
	CMuroaXml::prev();
}


void CConnection::doJoinSession(string name) {
	joinSession(name);

    m_sm.getLatestMediaCol();
    m_sm.getLatestPlaylist();
    m_sm.getLatestNextlist();

}


void CConnection::connected() {
    emit connectionStatusChanged( e_connected );
    CMuroaXml::open();

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
    emit connectionStatusChanged( e_disconnected );
}

void CConnection::error() {

}

void CConnection::readyRead() {
    int avail = m_socket.bytesAvailable();
    QByteArray data = m_socket.readAll();
    newData(data.data(), data.length());
}

void CConnection::sendCommand(CmdBase* cmd) {
    qDebug() << QString("sendCommand %1").arg(QString::fromUtf8(cmd->name().c_str()));

    switch(cmd->type()) {
		case CmdBase::GET_MEDIA_COL:
		{
			getCollection( cmd->knownRev() );
		}
		break;
		case CmdBase::GET_PLAYLIST:
		{
			getPlaylist( cmd->knownRev() );
		}
		break;
		case CmdBase::GET_NEXTLIST:
		{
			getNextlist( cmd->knownRev() );
		}
		break;
		case CmdBase::EDIT_MEDIA_COL:
		{
			CmdEditMediaCol* emc = static_cast<CmdEditMediaCol*>(cmd);
			editCollection( emc->knownRev(), emc->data() );
		}
		break;
		case CmdBase::EDIT_PLAYLIST:
		{

		}
		break;
		case CmdBase::EDIT_NEXTLIST:
		{

		}
		break;
		case CmdBase::PLAY:
		{

		} break;
		case CmdBase::PAUSE:
		{

		} break;
		case CmdBase::NEXT:
		{

		} break;
		case CmdBase::PREV:
		{

		} break;
		case CmdBase::SCAN_COLLECTION:
		{
			scanCollection(cmd->id());
		} break;
		default:
		{

		} break;
    }
    delete cmd;
}

