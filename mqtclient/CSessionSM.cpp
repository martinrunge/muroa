/*
 * CSessionSM.cpp
 *
 *  Created on: 28 Nov 2011
 *      Author: martin
 */

#include "CSessionSM.h"
#include "CConnection.h"
#include "cmds/CmdGetMediaCol.h"
#include "cmds/CmdGetPlaylist.h"
#include "cmds/CmdGetNextlist.h"
#include "cmds/CmdEditMediaCol.h"
#include "cmds/CmdEditPlaylist.h"
#include "cmds/CmdEditNextlist.h"
#include "cmds/CmdScanCollection.h"

CSessionSM::CSessionSM(CConnection* connection, QObject* parent) : m_connection(connection),
                                                                   m_knownMediaColRev(0),
                                                                   m_knownPlaylistRev(0),
                                                                   m_knownNextlistRev(0)
{

}

CSessionSM::~CSessionSM() {

}

void CSessionSM::getLatestMediaCol() {
	CmdGetMediaCol* cmd = new CmdGetMediaCol(getKnownMediaColRev());
	m_connection->sendCommand(cmd);
}

void CSessionSM::getLatestPlaylist() {
	CmdGetPlaylist* cmd = new CmdGetPlaylist(getKnownPlaylistRev());
	m_connection->sendCommand(cmd);
}

void CSessionSM::getLatestNextlist() {
	CmdGetNextlist* cmd = new CmdGetNextlist(getKnownNextlistRev());
	m_connection->sendCommand(cmd);
}

unsigned CSessionSM::getKnownMediaColRev() const
{
    return m_knownMediaColRev;
}

unsigned CSessionSM::getKnownNextlistRev() const
{
    return m_knownNextlistRev;
}

unsigned CSessionSM::getKnownPlaylistRev() const
{
    return m_knownPlaylistRev;
}

void CSessionSM::scanCollection() {
	CmdScanCollection* cmd = new CmdScanCollection(m_knownMediaColRev);

    m_connection->sendCommand(cmd);

}
