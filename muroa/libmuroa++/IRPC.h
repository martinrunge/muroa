/*
 * IRPC.h
 *
 *  Created on: 12 Aug 2011
 *      Author: martin
 */

#ifndef IRPC_H_
#define IRPC_H_

#include <string>
#include "MuroaExceptions.h"

class IRPC {
public:
//	IRPC() throw(rpcError);
	virtual ~IRPC() {}

	virtual void join(std::string host, unsigned port) = 0;
	virtual void leave() = 0;

	virtual void onConnectionStateChanged(int state) = 0;

	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void next() = 0;
	virtual void prev() = 0;

	virtual void getCollection( int knownRev ) = 0;
	virtual void getPlaylist( int knownRev ) = 0;
	virtual void getNextlist( int knownRev ) = 0;

	virtual void editCollection( int fromRev ) = 0;
	virtual void editPlaylist( int fromRev ) = 0;
	virtual void editNextlist( int fromRev ) = 0;

	virtual void onPlay() = 0;
	virtual void onStop() = 0;
	virtual void onNext() = 0;
	virtual void onPrev() = 0;

	virtual void onGetCollection( int knownRev ) = 0;
	virtual void onGetPlaylist( int knownRev ) = 0;
	virtual void onGetNextlist( int knownRev ) = 0;

	virtual void onEditCollection( int fromRev ) = 0;
	virtual void onEditPlaylist( int fromRev ) = 0;
	virtual void onEditNextlist( int fromRev ) = 0;

};

#endif /* IRPC_H_ */
