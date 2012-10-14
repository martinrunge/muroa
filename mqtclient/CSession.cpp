/*
 * CSession.cpp
 *
 *  Created on: 11 Feb 2012
 *      Author: martin
 */

#include "CSession.h"
#include "CMuroaTreeModel.h"
#include "CMuroaListModel.h"

#include <CRootItem.h>

#include <QSettings>
#include <QDesktopServices>
#include <QDir>

using namespace std;

CSession::CSession() : QObject(), m_connection(this) {
	m_mediaColModel = new CMuroaTreeModel();
	m_playlistModel = new CMuroaListModel();
	m_nextlistModel = new CMuroaListModel();

	m_sessionState = new muroa::CRootItem();

	m_playlistModel->setMediaCol(m_mediaColModel);
	m_nextlistModel->setMediaCol(m_mediaColModel);

	m_storeageLoc = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	#ifdef Q_WS_MAC
		m_storeageLoc.insert(m_storeageLoc.count() - QCoreApplication::applicationName().count(),
	                    QCoreApplication::organizationName() + "/");
	#endif
}

CSession::~CSession() {
	delete m_sessionState;

	delete m_nextlistModel;
	delete m_playlistModel;
	delete m_mediaColModel;
}


void CSession::openConnection(const CServiceDesc &sd) {
    m_connection.open(sd.getHostName(), sd.getPortNr());

	QSettings settings;
	settings.setValue("rejoin_service", sd.getServiceName());
	settings.setValue("rejoin_host", sd.getHostName());
	settings.setValue("rejoin_domain", sd.getDomainName());
}

void CSession::closeConnection() {
    m_connection.close();
}

void CSession::scanCollection() {
    m_connection.scanCollection(0);
}

void CSession::save()
{
	QDir sdir(m_storeageLoc);
	if(!sdir.exists()) {
		sdir.mkpath(m_storeageLoc);
	}
	string mediaColFile = QString("%1/%2").arg(m_storeageLoc).arg("mediacol.mcrev").toUtf8().data();
	m_mediaColModel->serialize(mediaColFile);
	string playlistFile = QString("%1/%2").arg(m_storeageLoc).arg("playlist.mcrev").toUtf8().data();
	m_playlistModel->getRootItem()->serialize(playlistFile);
	string nextlistFile = QString("%1/%2").arg(m_storeageLoc).arg("nextlist.mcrev").toUtf8().data();
	m_nextlistModel->getRootItem()->serialize(nextlistFile);
}

void CSession::restore()
{
	try {
		string mediaColFile = QString("%1/%2").arg(m_storeageLoc).arg("mediacol.mcrev").toUtf8().data();
		m_mediaColModel->fromFile(mediaColFile);
		string playlistFile = QString("%1/%2").arg(m_storeageLoc).arg("playlist.mcrev").toUtf8().data();
		m_playlistModel->getRootItem()->fromFile(playlistFile);
		string nextlistFile = QString("%1/%2").arg(m_storeageLoc).arg("nextlist.mcrev").toUtf8().data();
		m_nextlistModel->getRootItem()->fromFile(nextlistFile);
	}
	catch(ExMalformedPatch& ex) {

	}
}


void CSession::dumpCollection() {
	string mediaCol;
	mediaCol = m_mediaColModel->serialize();
	cout << mediaCol;
}
