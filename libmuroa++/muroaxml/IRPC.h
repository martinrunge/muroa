/*
 * IRPC.h
 *
 *  Created on: 12 Aug 2011
 *      Author: martin
 */

#ifndef IRPC_H_
#define IRPC_H_

#include <string>
#include <vector>
#include "MuroaExceptions.h"

class IRPC {
public:
//	IRPC() throw(rpcError);
	virtual ~IRPC() {}

	virtual void open() = 0;
	virtual void close() = 0;

	virtual void listSessions(std::vector<std::string> sessionList = std::vector<std::string>()) = 0;
	virtual void onListSessions(std::vector<std::string> sessionList) = 0;

	virtual void joinSession(std::string sessionName) = 0;
	virtual void onJoinSession(std::string sessionName) = 0;

	virtual void leaveSession() = 0;
	virtual void onLeaveSession() = 0;

	virtual void play(uint32_t jobID) = 0;
	virtual void stop(uint32_t jobID) = 0;
	virtual void next(uint32_t jobID) = 0;
	virtual void prev(uint32_t jobID) = 0;

	virtual void stateChanged(int newState) = 0;
	virtual void onStateChanged(int newState) = 0;

	virtual void scanCollection(uint32_t jobID) = 0;
	virtual void onScanCollection(uint32_t jobID) = 0;

	virtual void progress(uint32_t jobID, int progress, int total) = 0;
	virtual void onProgress(uint32_t jobID, int progress, int total) = 0;

	virtual void finished(uint32_t jobID) = 0;
	virtual void onFinished(uint32_t jobID) = 0;

	virtual void error(uint32_t jobID, int errorCode, std::string description) = 0;
	virtual void onError(uint32_t jobID, int errorCode, std::string description) = 0;

	virtual void getCollection(uint32_t jobID, unsigned knownRev ) = 0;
	virtual void getPlaylist(uint32_t jobID, unsigned knownRev ) = 0;
	virtual void getNextlist(uint32_t jobID, unsigned knownRev ) = 0;

	virtual void editCollection(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string diff ) = 0;
	virtual void editPlaylist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string diff ) = 0;
	virtual void editNextlist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string diff ) = 0;

	virtual void onPlay(uint32_t jobID = 0) = 0;
	virtual void onStop(uint32_t jobID = 0) = 0;
	virtual void onNext(uint32_t jobID = 0) = 0;
	virtual void onPrev(uint32_t jobID = 0) = 0;

	virtual void onGetCollection(uint32_t jobID, unsigned knownRev ) = 0;
	virtual void onGetPlaylist(uint32_t jobID, unsigned knownRev ) = 0;
	virtual void onGetNextlist(uint32_t jobID, unsigned knownRev ) = 0;

	virtual void onEditCollection(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string collectionDiff) = 0;
	virtual void onEditPlaylist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string playlistDiff ) = 0;
	virtual void onEditNextlist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string nextlistDiff ) = 0;

	// onDataToSend:
	// called when there are serialized command ready to be sent to receiver
	virtual void onDataToSend(const char* data, int len) = 0;

	// newData:
	// when data is received from pther end, feed it in here. The on... Methonds
	// will be called if a complete command was contained in the data.
	virtual void newData(const char* data, int len) = 0;

};

#endif /* IRPC_H_ */
