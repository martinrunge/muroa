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

using namespace std;

CSession::CSession() : QObject(), m_connection(this) {
	m_mediaColModel = new CMuroaTreeModel();
	m_playlistModel = new CMuroaListModel();
	m_nextlistModel = new CMuroaListModel();
}

CSession::~CSession() {
	// TODO Auto-generated destructor stub
}


CMuroaTreeModel* CSession::getMediaColModel() const {
	return m_mediaColModel;
}

CMuroaListModel* CSession::getNextlistModel() const {
	return m_playlistModel;
}

CMuroaListModel* CSession::getPlaylistModel() const {
	return m_nextlistModel;
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

uint32_t CSession::getMediaColRev() const {
	return m_mediaColRev;
}

void CSession::setMediaColRev(const uint32_t mediaColRev) {
	m_mediaColRev = mediaColRev;
}

uint32_t CSession::getPlaylistRev() const {
	return m_mediaColRev;
}

void CSession::setPlaylistRev(const uint32_t nextlistRev) {
	m_playlistRev = nextlistRev;
}

uint32_t CSession::getNextlistRev() const {
	return m_mediaColRev;
}

void CSession::setNextlistRev(const uint32_t playlistRev) {
	m_nextlistRev = playlistRev;
}

void CSession::dumpCollection() {
	string mediaCol;
	mediaCol = m_mediaColModel->serialize();
	cout << mediaCol;
}
