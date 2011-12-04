/*
 * CPlaylistCommand.cpp
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#include "CmdEditPlaylist.h"

using namespace std;

CmdEditPlaylist::CmdEditPlaylist(unsigned knownRev, QObject * parent) :  CmdBase(EDIT_PLAYLIST, knownRev, parent) {
	m_name = "editPlaylist";
}

CmdEditPlaylist::CmdEditPlaylist(unsigned knownRev, const string& diff, QObject * parent) :  CmdBase(EDIT_PLAYLIST, knownRev, parent) {
	m_name = "editPlaylist";
	m_data = diff;
}

CmdEditPlaylist::~CmdEditPlaylist() {

}

void CmdEditPlaylist::setData(const string& diff)
{
	m_data = diff;
}

void CmdEditPlaylist::timeout()
{
	CmdBase::timeout();
}
