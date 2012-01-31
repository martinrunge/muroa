/*
 * CRpc.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CRpc.h"
#include "CConnection.h"
#include "CSessionContainer.h"
#include "CSession.h"

#include <vector>
#include <string>

using namespace std;

namespace muroa {

CRpc::CRpc(CConnection* connection) : m_connection(connection){
	open();
}

CRpc::~CRpc() {
	// close();
}

    void CRpc::onDataToSend(const char *data, int length) {
    	m_connection->writeData(data, length);
    }

	void CRpc::onListSessions(vector<string> sessions) {
		vector<string> knownSessions = CSessionContainer::getInstPtr()->listSessions();
		listSessions(knownSessions);
	}

	void CRpc::onJoinSession(string name) {
		m_connection->joinSession(name);
    }

    void CRpc::onLeaveSession()
    {
    }

    void CRpc::onPlay()
    {
    }

    void CRpc::onPause()
    {
    }

    void CRpc::onStop()
    {
    }

    void CRpc::onNext()
    {
    }

    void CRpc::onPrev()
    {
    }

    void CRpc::onStateChanged(int newState)
    {
    }

    void CRpc::onScanCollection(uint32_t jobID) {
    	// scan collection and give progress reports
    	m_connection->getSession()->scanCollection(m_connection, jobID);
    }

    void CRpc::onProgress(uint32_t jobID, int progress)
    {
    }

	void CRpc::onFinished(uint32_t jobID) {
	}


    void CRpc::onError(uint32_t jobID, int errorCode, std::string description)
    {
    }

    void CRpc::onGetCollection(unsigned  knownRev) {
    	m_connection->sendLatestMediaColRev(knownRev);
    }

    void CRpc::onGetPlaylist(unsigned  knownRev) {
    	m_connection->sendLatestPlaylistRev(knownRev);
    }

    void CRpc::onGetNextlist(unsigned  knownRev) {
    	m_connection->sendLatestNextlistRev(knownRev);
    }

    void CRpc::onCollection(unsigned  diffFromRev, std::string collection)
    {
    }

    void CRpc::onPlaylist(unsigned  diffFromRev, std::string playlist)
    {
    }

    void CRpc::onNextlist(unsigned  diffFromRev, std::string nextlist)
    {
    }

    void CRpc::onEditCollection(unsigned  fromRev, std::string collectionDiff)
    {
    }

    void CRpc::onEditPlaylist(unsigned  fromRev, std::string playlistDiff)
    {
    }

    void CRpc::onEditNextlist(unsigned  fromRev, std::string nextlistDiff)
    {}

} /* namespace muroa */
