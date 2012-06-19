/***************************************************************************
 *
 *   CRpcDummy.h
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

#ifndef CRPCDUMMY_H_
#define CRPCDUMMY_H_

#include "muroaxml/CMuroaXml.h"
#include <vector>
#include <string>

class CRpcDummy: public CMuroaXml {
public:
	CRpcDummy(CRpcDummy* receiver = 0);
	virtual ~CRpcDummy();

	std::string getLastCmd();
    std::vector<std::string> getLastStringVec();

	void onDataToSend(const char* data, int length);

	void onListSessions(std::vector<std::string> sessionList);
	void onJoinSession(std::string sessionName);
	void onLeaveSession();

	void onPlay(uint32_t jobID);
	void onPause(uint32_t jobID);
	void onStop(uint32_t jobID);
	void onNext(uint32_t jobID);
	void onPrev(uint32_t jobID);

	void onStateChanged(int newState);
	void onScanCollection(uint32_t jobID);
	void onProgress(uint32_t jobID, int progress);
	void onFinished(uint32_t jobID);
	void onError(uint32_t jobID, int errorCode, std::string description);

	void onGetCollection(uint32_t jobID, unsigned knownRev );
	void onGetPlaylist(uint32_t jobID, unsigned knownRev );
	void onGetNextlist(uint32_t jobID, unsigned knownRev );

	void onCollection(uint32_t jobID, unsigned  diffFromRev, std::string collection);
	void onPlaylist(uint32_t jobID, unsigned  diffFromRev, std::string playlist);
	void onNextlist(uint32_t jobID, unsigned  diffFromRev, std::string nextlist);

	void onEditCollection(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string collectionDiff );
	void onEditPlaylist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string playlistDiff );
	void onEditNextlist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string nextlistDiff );

	void onStartSession(){ };
	void onEndSession(){ };
	void onXmlVersion(){ };

private:
	CRpcDummy* m_receiver;
	std::string m_last_cmd;
	std::vector<std::string> m_last_stringvec;
};

#endif /* CRPCDUMMY_H_ */
