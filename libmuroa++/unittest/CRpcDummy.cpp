/***************************************************************************
 *
 *   CRpcDummy.cpp
 *
 *   This file is part of libmuroa++                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "CRpcDummy.h"

#include <sstream>

using namespace std;

CRpcDummy::CRpcDummy(CRpcDummy* receiver) : m_receiver(receiver) {
}

CRpcDummy::~CRpcDummy() {
}

string CRpcDummy::getLastCmd() {
	return m_last_cmd;
}

vector<string> CRpcDummy::getLastStringVec() {
	return m_last_stringvec;
}

void CRpcDummy::onDataToSend(const char* data, int length) {
	m_receiver->newData(data, length);
}

void CRpcDummy::onPlay() {
	m_last_cmd = "play";
}

void CRpcDummy::onPause() {
	m_last_cmd = "pause";
}

void CRpcDummy::onStop() {
	m_last_cmd = "stop";
}

void CRpcDummy::onNext() {
	m_last_cmd = "next";
}

void CRpcDummy::onPrev() {
	m_last_cmd = "prev";
}

void CRpcDummy::onStateChanged(int newState) {
	ostringstream oss;
	oss << "stateChanged(" << newState << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onScanCollection(uint32_t jobID) {
	ostringstream oss;
	oss << "progress(" << jobID << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onProgress(uint32_t jobID, int progress) {
	ostringstream oss;
	oss << "progress(" << jobID << "," << progress << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onFinished(uint32_t jobID) {
	ostringstream oss;
	oss << "finished(" << jobID << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onError(uint32_t jobID, int errorCode, std::string description) {
	ostringstream oss;
	oss << "error(" << jobID << "," << errorCode << ",\"" << description << "\")";
	m_last_cmd = oss.str();

}

void CRpcDummy::onGetCollection(unsigned  knownRev) {
	ostringstream oss;
	oss << "getCollection(" << knownRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onGetPlaylist(unsigned  knownRev) {
	ostringstream oss;
	oss << "getPlaylist(" << knownRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onGetNextlist(unsigned  knownRev) {
	ostringstream oss;
	oss << "getNextlist(" << knownRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onCollection(unsigned  diffFromRev, string collection) {
	ostringstream oss;
	oss << "collection(" << diffFromRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onPlaylist(unsigned  diffFromRev, string playlist) {
	ostringstream oss;
	oss << "playlist(" << diffFromRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onNextlist(unsigned  diffFromRev, string nextlist) {
	ostringstream oss;
	oss << "nextlist(" << diffFromRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onEditCollection(unsigned  fromRev, unsigned toRev, string collectionDiff) {
	ostringstream oss;
	oss << "editCollection(" << fromRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onEditPlaylist(unsigned  fromRev, unsigned toRev, string playlistDiff) {
	ostringstream oss;
	oss << "editPlaylist(" << fromRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onEditNextlist(unsigned  fromRev, unsigned toRev, string nextlistDiff) {
	ostringstream oss;
	oss << "editNextlist(" << fromRev << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onListSessions(std::vector<std::string> sessionList) {
	m_last_stringvec = sessionList;
	m_last_cmd = "sessionList";
}

void CRpcDummy::onJoinSession(string  sessionName) {
	ostringstream oss;
	oss << "joinSession((" << sessionName << ")";
	m_last_cmd = oss.str();
}

void CRpcDummy::onLeaveSession() {
	m_last_cmd = "leaveSession";
}



