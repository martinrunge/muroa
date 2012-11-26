/**
 * \file CSession.h
 *
 *  \date 31 Oct 2011
 *  \author  Martin Runge <martin dot runge at web dot de>
 */

#ifndef CSESSION_H_
#define CSESSION_H_

#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <CConnection.h>
#include <CRootItem.h>
#include <MuroaExceptions.h>

#include <avahi/CServiceDesc.h>
#include <mediaprocessing/CStream.h>

#include <boost/asio.hpp>

#include "sessionEx.h"
#include "CPlaylistIdProvider.h"
#include "CStreamClientHdl.h"

class CMsgBase;
class CCmdDispatcher;


namespace muroa {

class CPlaylistItem;
class CNextlistItem;

class Cmd;
class CTcpServer;
class CMediaScannerCtrl;
class CApp;
class CStateDB;
class CSessionStorage;


class CSession : boost::noncopyable {
public:
	CSession(std::string name, boost::asio::io_service& io_service, CSessionContainer* const sessionContainer);
 	//CSession( std::string name );
	virtual ~CSession();

	inline boost::asio::io_service& getIoService() {return m_io_service; };
	std::string getName() { return m_name; };

	void play();
	void pause();
	void stop();
	void prev();
	void next();

	CMediaItem* getCurrentMediaItem() throw(ExInvMsg);

	void addConnection(CConnection* ptr);
	void removeConnection(CConnection* ptr);

	CRootItem*  getMediaCol(int revision = -1) const throw(ExInvMsg);
	CRootItem*  getPlaylist(int revision = -1) const throw(ExInvMsg);
	CRootItem*  getNextlist(int revision = -1) const throw(ExInvMsg);
	CRootItem*  getSessionState(int revision = -1) const throw(ExInvMsg);

	const std::string getMediaColDiff(unsigned fromRevision, int toRevision = -1) const;
	const std::string getPlaylistDiff(unsigned fromRevision, int toRevision = -1) const;
	const std::string getNextlistDiff(unsigned fromRevision, int toRevision = -1) const;
	const std::string getSessionStateDiff(unsigned fromRevision, int toRevision = -1) const;

	inline int getMaxMediaColRev() const { return m_maxMediaColRev; };
	inline int getMaxPlaylistRev() const { return m_maxPlaylistRev; };
	inline int getMaxNextlistRev() const { return m_maxNextlistRev; };
	inline int getMaxSessionStateRev() const { return m_maxSessionStateRev; };

	inline int getMinMediaColRev() const { return m_minMediaColRev; };
	inline int getMinPlaylistRev() const { return m_minPlaylistRev; };
	inline int getMinNextlistRev() const { return m_minNextlistRev; };
	inline int getMinSessionStateRev() const { return m_minSessionStateRev; };

	inline bool hasMediaColRev(unsigned rev) const
	{
		return (rev != 0 && rev <= m_maxMediaColRev && rev >= m_minMediaColRev);
	};

	inline bool hasPlaylistRev(unsigned rev) const
	{
		return (rev != 0 && rev <= m_maxPlaylistRev && rev >= m_minPlaylistRev);
	};

	inline bool hasNextlistRev(unsigned rev) const
	{
		return (rev != 0 && rev <= m_maxNextlistRev && rev >= m_minNextlistRev);
	};

	inline bool hasSessionStateRev(unsigned rev) const
	{
		return (rev != 0 && rev <= m_maxSessionStateRev && rev >= m_minSessionStateRev);
	};


	inline void setMinMediaColRev( const unsigned min ) { m_minMediaColRev = min; };
	inline void setMinPlaylistRev( const unsigned min ) { m_minPlaylistRev = min; };
	inline void setMinNextlistRev( const unsigned min ) { m_minNextlistRev = min; };
	inline void setMinSessionStateRev( const unsigned min ) { m_minSessionStateRev = min; };

	inline void setMaxMediaColRev( const unsigned max ) { m_maxMediaColRev = max; };
	inline void setMaxPlaylistRev( const unsigned max ) { m_maxPlaylistRev = max; };
	inline void setMaxNextlistRev( const unsigned max ) { m_maxNextlistRev = max; };
	inline void setMaxSessionStateRev( const unsigned max ) { m_maxSessionStateRev = max; };


	void addMediaColRev(CRootItem* ri);
	void addMediaColRev(const std::string& mediaCol );
	void addPlaylistRev(CRootItem* ri);
	void addPlaylistRev(const std::string& playlist);
	void addNextlistRev(CRootItem* ri);
	void addNextlistRev(const std::string& nextlist);
	void addSessionStateRev(CRootItem* ri);
	void addSessionStateRev(const std::string& sessionState);

	int addMediaColRevFromDiff(const std::string& mediaColDiff, unsigned diffFromRev) throw(ExInvMsg);
	int addPlaylistRevFromDiff(const std::string& playlistDiff, unsigned diffFromRev) throw(ExInvMsg);
	int addNextlistRevFromDiff(const std::string& nextlistDiff, unsigned diffFromRev) throw(ExInvMsg);
	int addSessionStateRevFromDiff(const std::string& sessionStateDiff, unsigned diffFromRev) throw(ExInvMsg);

	int addNextlistRevFromNextCmd();
	int addNextlistRevFromPrevCmd();

	void scanCollection(CConnection* initiator, uint32_t jobID);
	void scanProgress(uint32_t jobID, uint32_t progress);
	void jobFinished(uint32_t jobID);
	void collectionChanged(uint32_t newRev, uint32_t minRev, uint32_t maxRev);
	void response(uint32_t requestID, int32_t returnCode, std::string message);
	void reportError(uint32_t jobID, int32_t errCode, std::string message);

	void toAll( Cmd* cmd, bool deleteCmd = true );
	void sendToInitiator(Cmd* cmd, unsigned connId);
	void incomingCmd(Cmd*  cmd, CConnection* initiator);
	void postIncomingCmd(Cmd* cmd);

	std::string getProperty(std::string key, std::string defaultVal = "");
	void setProperty(std::string key, std::string val);

	int getProperty(std::string key, int defaultVal = 0);
	void setProperty(std::string key, int val);

	bool operator==(const CSession& other);
	inline bool operator!=(const CSession& other) { return !operator==(other); }

	void addClient(std::string name);
	void rmClient(std::string name);
	void takeClient(std::string name, std::string ownerSessionsName);

	ServDescPtr getServiceByName(std::string name);


private:

	typedef std::pair<CConnection*, uint32_t> client_job_t;

	bool hasConnection(CConnection* conn);

	CRootItem* getRev(const std::map<unsigned, CRootItem*>& mabp,
			          const unsigned rev,
			          const std::string& message) const throw(ExInvRev);

	boost::asio::io_service& m_io_service;

	// CTcpServer* m_tcp_server;
	std::string m_name;
	std::set<CConnection*> m_connections;
	std::map<unsigned, CConnection*> m_connections_by_id;

	CStreamClientHdl m_streamClientHdl;

	std::map<uint32_t, CConnection*> m_job_initiators;

	client_job_t getClientCmdIdBySubprocessCmdID(uint32_t subprocess_cmd_id, bool delentry = true) throw(ExInvMsg);
	void setClientCmdIdBySubprocessCmdID(uint32_t subprocess_cmd_id, CConnection* initiator, uint32_t client_cmd_id );

	std::map<uint32_t, std::pair<CConnection*, uint32_t> > m_subprocess_job_by_cmdID;

	void addOutstandingMsg(CMsgBase* msg);
	void delOutstandingMsg(uint32_t id) throw(ExInvMsg);

	std::map<uint32_t, CMsgBase*> m_outstanding_msgs;


	std::map<unsigned, CRootItem*> m_mediaColRevs;
	std::map<unsigned, CRootItem*> m_playlistRevs;
	std::map<unsigned, CRootItem*> m_nextlistRevs;
	std::map<unsigned, CRootItem*> m_sessionStateRevs;

    unsigned m_maxMediaColRev;
    unsigned m_maxPlaylistRev;
    unsigned m_maxNextlistRev;
    unsigned m_maxSessionStateRev;

    unsigned m_minMediaColRev;
    unsigned m_minPlaylistRev;
    unsigned m_minNextlistRev;
    unsigned m_minSessionStateRev;

    unsigned m_playlistPos;

    std::string m_stateDBFilename;
    CStateDB* m_stateDB;

    CStream m_stream;

	CSessionStorage* m_sessionStorage;

    // boost::asio::io_service& m_io_service;
    CMediaScannerCtrl* m_mediaScanner;
    CCmdDispatcher* m_cmdDispatcher;

    CPlaylistIdProvider m_plIdProvider;

    CApp* m_app;
    CSessionContainer* const m_sessionContainer;
    std::string privatePropertyKey(std::string key);

    void dequeueCmd();
    std::queue<Cmd*> m_cmd_queue;

    void dumpLookupErrorToLog(std::string descr, CPlaylistItem* plItem, CNextlistItem* nlItem);
};

} /* namespace muroa */
#endif /* CSESSION_H_ */
