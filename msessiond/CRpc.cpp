/*
 * CRpc.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CRpc.h"

namespace muroa {

CRpc::CRpc() {
	// TODO Auto-generated constructor stub

}

CRpc::~CRpc() {
	// TODO Auto-generated destructor stub
}

    void CRpc::onDataToSend(const char *data, int length)
    {
    }

    void CRpc::onJoinSession(uint32_t sessionID)
    {
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

    void CRpc::onProgress(uint32_t jobID, int progress)
    {
    }

    void CRpc::onError(uint32_t jobID, int errorCode, std::string description)
    {
    }

    void CRpc::onGetCollection(unsigned  knownRev)
    {
    }

    void CRpc::onGetPlaylist(unsigned  knownRev)
    {
    }

    void CRpc::onGetNextlist(unsigned  knownRev)
    {
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
