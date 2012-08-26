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

	void onPlay(uint32_t jobID);
	void onPause(uint32_t jobID);
	void onStop(uint32_t jobID);
	void onNext(uint32_t jobID);
	void onPrev(uint32_t jobID);

	void onStateChanged(int newState);
	void onScanCollection( uint32_t jobID);

	void onProgress( uint32_t jobID, int progress, int total);
	void onFinished( uint32_t jobID);
	void onError( uint32_t jobID, int errorCode, std::string description);

	void onGetCollection( uint32_t jobID, unsigned knownRev );
	void onGetPlaylist( uint32_t jobID, unsigned knownRev );
	void onGetNextlist( uint32_t jobID, unsigned knownRev );

	void onCollection( uint32_t jobID, unsigned  diffFromRev, std::string collection);
	void onPlaylist( uint32_t jobID, unsigned  diffFromRev, std::string playlist);
	void onNextlist( uint32_t jobID, unsigned  diffFromRev, std::string nextlist);

	void onEditCollection( uint32_t jobID, unsigned fromRev, unsigned toRev, std::string collectionDiff );
	void onEditPlaylist( uint32_t jobID, unsigned fromRev, unsigned toRev, std::string playlistDiff );
	void onEditNextlist( uint32_t jobID, unsigned fromRev, unsigned toRev, std::string nextlistDiff );

	void onGetSessionClients(uint32_t jobID);
	void onGetUnassignedClients(uint32_t jobID);
	void onAddClient(uint32_t jobID, std::string name);
	void onRmClient(uint32_t jobID, std::string name);
	void onEnableClient(uint32_t jobID, std::string name);
	void onDisableClient(uint32_t jobID, std::string name);


	void onStartSession(){ };
	void onEndSession(){ };
	void onXmlVersion(){ };

	void sendCmd(muroa::Cmd* cmd);

private:
	CConnection* m_connection;

};

} /* namespace muroa */
#endif /* CRPC_H_ */
