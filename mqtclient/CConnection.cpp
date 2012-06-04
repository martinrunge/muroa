#include "CConnection.h"
#include "cmds/CmdBase.h"
#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"
#include "muroaConstants.h"

#include "CSession.h"
#include "CMuroaListModel.h"
#include "CMuroaTreeModel.h"

#include <MuroaExceptions.h>

#include <QDebug>

using namespace std;

CConnection::CConnection(CSession* const session) : m_sm(this), m_session(session) {
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

void CConnection::onFinished(uint32_t jobID) {

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

void CConnection::onEditCollection(unsigned  fromRev, unsigned toRev, std::string collectionDiff) {
	try {
		if(fromRev == 0) {
			try {
				m_session->getMediaColModel()->deserialize(collectionDiff);
			}
			catch(MalformedPatchEx ex) {
				;
			}
		}
		else {
			uint32_t knownRev = m_session->getMediaColModel()->getRevision();
			if( knownRev != fromRev ) {
				std::ostringstream oss;
				oss << "editCollection: Error: got a diff based on rev " << fromRev << " but known rev is " << knownRev;
				throw MalformedPatchEx(oss.str(), 0);
			}
			m_session->getMediaColModel()->patch(collectionDiff);
		}
		m_session->getMediaColModel()->setRevision(toRev);
	}
	catch(MalformedPatchEx& ex)
	{

	}
}

void CConnection::onEditPlaylist(unsigned  fromRev, unsigned toRev, std::string playlistDiff) {
	try {
		if(fromRev == 0) {
			m_session->getPlaylistModel()->deserialize(playlistDiff);
		}
		else {
			uint32_t knownRev = m_session->getPlaylistModel()->getRevision();
			if( knownRev != fromRev ) {
				std::ostringstream oss;
				oss << "editPlaylist: Error: got a diff based on rev " << fromRev << " but known rev is " << knownRev;
				throw MalformedPatchEx(oss.str(), 0);
			}
			m_session->getPlaylistModel()->patch(playlistDiff);
		}
		m_session->getPlaylistModel()->setRevision(toRev);
	}
	catch(MalformedPatchEx& ex)
	{

	}
}

void CConnection::onEditNextlist(unsigned  fromRev, unsigned toRev, std::string nextlistDiff) {
	try {
		if(fromRev == 0) {
			m_session->getNextlistModel()->deserialize(nextlistDiff);
		}
		else {
			uint32_t knownRev = m_session->getNextlistModel()->getRevision();
			if( knownRev != fromRev ) {
				std::ostringstream oss;
				oss << "editNextlist: Error: got a diff based on rev " << fromRev << " but known rev is " << knownRev;
				throw MalformedPatchEx(oss.str(), 0);
			}
			m_session->getNextlistModel()->patch(nextlistDiff);
		}
		m_session->getNextlistModel()->setRevision(toRev);
	}
	catch(MalformedPatchEx& ex)
	{

	}
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
	CMuroaXml::close();
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
			editCollection( emc->knownRev(), 0, emc->data() );
		}
		break;
		case CmdBase::EDIT_PLAYLIST:
		{
			CmdEditPlaylist* epl = static_cast<CmdEditPlaylist*>(cmd);
			editPlaylist( epl->knownRev(), 0, epl->data() );
		}
		break;
		case CmdBase::EDIT_NEXTLIST:
		{
			CmdEditNextlist* enl = static_cast<CmdEditNextlist*>(cmd);
			editNextlist( enl->knownRev(), 0, enl->data() );
		}
		break;
		case CmdBase::PLAY:
		{
			play();
		} break;
		case CmdBase::PAUSE:
		{
			pause();
		} break;
		case CmdBase::NEXT:
		{
			next();
		} break;
		case CmdBase::PREV:
		{
			prev();
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

