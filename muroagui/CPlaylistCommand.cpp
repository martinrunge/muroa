/*
 * CPlaylistCommand.cpp
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#include "CPlaylistCommand.h"

CPlaylistCommand::CPlaylistCommand(int knownRev, QObject * parent) :  CCommandBase(knownRev, parent) {
	m_cmdName = "modPlaylist";
}

CPlaylistCommand::CPlaylistCommand(int knownRev, const QString& diff, QObject * parent) :  CCommandBase(knownRev, parent) {
	m_cmdName = "modPlaylist";
	m_cmdData = diff;
}

CPlaylistCommand::~CPlaylistCommand() {

}

void CPlaylistCommand::setData(const QString& diff)
{
	m_cmdData = diff;
}

void CPlaylistCommand::timeout()
{
	CCommandBase::timeout();
}
