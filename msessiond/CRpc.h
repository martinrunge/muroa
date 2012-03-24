/*
 * CRpc.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CRPC_H_
#define CRPC_H_

#include <muroaxml/CMuroaXml.h>
#include "sessionEx.h"

namespace muroa {

class CConnection;
class Cmd;

class CRpc: public CMuroaXml {
public:
	CRpc(CConnection* connection);
	virtual ~CRpc();

	void onDataToSend(const char* data, int length);
	void onListSessions(std::vector<std::string> sessions);

	void onJoinSession(std::string name);
	void onLeaveSession();

	void onPlay();
	void onPause();
	void onStop();
	void onNext();
	void onPrev();

	void onStateChanged(int newState);
	void onScanCollection(uint32_t jobID);

	void onProgress(uint32_t jobID, int progress);
	void onFinished(uint32_t jobID);
	void onError(uint32_t jobID, int errorCode, std::string description);

	void onGetCollection( unsigned knownRev );
	void onGetPlaylist( unsigned knownRev );
	void onGetNextlist( unsigned knownRev );

	void onCollection(unsigned  diffFromRev, std::string collection);
	void onPlaylist(unsigned  diffFromRev, std::string playlist);
	void onNextlist(unsigned  diffFromRev, std::string nextlist);

	void onEditCollection( unsigned fromRev, unsigned toRev, std::string collectionDiff );
	void onEditPlaylist( unsigned fromRev, unsigned toRev, std::string playlistDiff );
	void onEditNextlist( unsigned fromRev, unsigned toRev, std::string nextlistDiff );

	void onStartSession(){ };
	void onEndSession(){ };
	void onXmlVersion(){ };

	void sendCmd(muroa::Cmd* cmd);

private:
	CConnection* m_connection;

};

} /* namespace muroa */
#endif /* CRPC_H_ */
