/*
 * CSessionSM.cpp
 *
 *  Created on: 28 Nov 2011
 *      Author: martin
 */

#include "CSession.h"
#include "CSessionSM.h"
#include "CConnection.h"
#include "cmds/CmdGetMediaCol.h"
#include "cmds/CmdGetPlaylist.h"
#include "cmds/CmdGetNextlist.h"
#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"
#include "cmds/CmdScanCollection.h"

CSessionSM::CSessionSM(CConnection* connection, QObject* parent) : m_connection(connection)
{

}

CSessionSM::~CSessionSM()
{

}

void CSessionSM::getLatestMediaCol() {
	unsigned knownMediaColRev = m_connection->getSession()->getMediaColRev();
	CmdGetMediaCol* cmd = new CmdGetMediaCol(knownMediaColRev);
	m_connection->sendCommand(cmd);
}

void CSessionSM::getLatestPlaylist() {
	unsigned knownPlaylistRev = m_connection->getSession()->getPlaylistRev();
	CmdGetPlaylist* cmd = new CmdGetPlaylist(knownPlaylistRev);
	m_connection->sendCommand(cmd);
}

void CSessionSM::getLatestNextlist() {
	unsigned knownNextlistRev = m_connection->getSession()->getNextlistRev();
	CmdGetNextlist* cmd = new CmdGetNextlist(knownNextlistRev);
	m_connection->sendCommand(cmd);
}


void CSessionSM::scanCollection() {
	unsigned knownMediaColRev = m_connection->getSession()->getMediaColRev();
	CmdScanCollection* cmd = new CmdScanCollection(knownMediaColRev);

    m_connection->sendCommand(cmd);

}
