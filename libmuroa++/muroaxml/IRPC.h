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

	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void next() = 0;
	virtual void prev() = 0;

	virtual void stateChanged(int newState) = 0;
	virtual void onStateChanged(int newState) = 0;

	virtual void scanCollection(uint32_t jobID) = 0;
	virtual void onScanCollection(uint32_t jobID) = 0;

	virtual void progress(uint32_t jobID, int progress) = 0;
	virtual void onProgress(uint32_t jobID, int progress) = 0;

	virtual void error(uint32_t jobID, int errorCode, std::string description) = 0;
	virtual void onError(uint32_t jobID, int errorCode, std::string description) = 0;

	virtual void getCollection( unsigned knownRev ) = 0;
	virtual void getPlaylist( unsigned knownRev ) = 0;
	virtual void getNextlist( unsigned knownRev ) = 0;

	virtual void editCollection( unsigned fromRev, std::string diff ) = 0;
	virtual void editPlaylist( unsigned fromRev, std::string diff ) = 0;
	virtual void editNextlist( unsigned fromRev, std::string diff ) = 0;

	virtual void onPlay() = 0;
	virtual void onStop() = 0;
	virtual void onNext() = 0;
	virtual void onPrev() = 0;

	virtual void onGetCollection( unsigned knownRev ) = 0;
	virtual void onGetPlaylist( unsigned knownRev ) = 0;
	virtual void onGetNextlist( unsigned knownRev ) = 0;

	virtual void onEditCollection( unsigned fromRev, std::string collectionDiff) = 0;
	virtual void onEditPlaylist( unsigned fromRev, std::string playlistDiff ) = 0;
	virtual void onEditNextlist( unsigned fromRev, std::string nextlistDiff ) = 0;

	// onDataToSend:
	// called when there are serialized command ready to be sent to receiver
	virtual void onDataToSend(const char* data, int len) = 0;

	// newData:
	// when data is received from pther end, feed it in here. The on... Methonds
	// will be called if a complete command was contained in the data.
	virtual void newData(const char* data, int len) = 0;

};

#endif /* IRPC_H_ */
