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
#include <CConnection.h>
#include <CRootItem.h>

#include "sessionEx.h"

namespace muroa {

class Cmd;
class CTcpServer;
class CMediaScannerCtrl;
class CApp;
class CStateDB;

class CSession : boost::noncopyable {
public:
	CSession(std::string name, boost::asio::io_service& io_service);
 	CSession( std::string name );
	virtual ~CSession();

	std::string getName() { return m_name; };

	void addConnection(CConnection* ptr);
	void removeConnection(CConnection* ptr);

	CRootItem*  getMediaCol(int revision = -1) const;
	CRootItem*  getPlaylist(int revision = -1) const;
	CRootItem*  getNextlist(int revision = -1) const;

	const std::string getMediaColDiff(unsigned fromRevision, int toRevision = -1) const;
	const std::string getPlaylistDiff(unsigned fromRevision, int toRevision = -1) const;
	const std::string getNextlistDiff(unsigned fromRevision, int toRevision = -1) const;

	inline int getMaxMediaColRev() const { return m_maxMediaColRev; };
	inline int getMaxPlaylistRev() const { return m_maxPlaylistRev; };
	inline int getMaxNextlistRev() const { return m_maxNextlistRev; };

	inline int getMinMediaColRev() const { return m_minMediaColRev; };
	inline int getMinPlaylistRev() const { return m_minPlaylistRev; };
	inline int getMinNextlistRev() const { return m_minNextlistRev; };

	inline void setMinMediaColRev( const unsigned min ) { m_minMediaColRev = min; };
	inline void setMinPlaylistRev( const unsigned min ) { m_minPlaylistRev = min; };
	inline void setMinNextlistRev( const unsigned min ) { m_minNextlistRev = min; };


	void addMediaColRev(CRootItem* ri);
	void addMediaColRev(const std::string& mediaCol );
	void addPlaylistRev(CRootItem* ri);
	void addPlaylistRev(const std::string& playlist);
	void addNextlistRev(CRootItem* ri);
	void addNextlistRev(const std::string& nextlist);

	int addMediaColRevFromDiff(const std::string& mediaColDiff, unsigned diffFromRev) throw(InvalidMsgException);
	int addPlaylistRevFromDiff(const std::string& playlistDiff, unsigned diffFromRev) throw(InvalidMsgException);
	int addNextlistRevFromDiff(const std::string& nextlistDiff, unsigned diffFromRev) throw(InvalidMsgException);

	int addNextlistRevFromNextCmd();
	int addNextlistRevFromPrevCmd();

	void setMinMediaColRevision(int rev) throw();
	void setMinPlaylistRevision(int rev) throw();
	void setMinNextlistRevision(int rev) throw();

	void scanCollection(uint32_t jobID);
	void scanProgress(uint32_t jobID, uint32_t progress);
	void jobFinished(uint32_t jobID);
	void collectionChanged(uint32_t newRev, uint32_t minRev, uint32_t maxRev);
	void response(uint32_t requestID, int32_t returnCode, std::string message);
	void reportError(uint32_t jobID, int32_t errCode, std::string message);

	void toAll( Cmd* cmd );

	std::string getProperty(std::string key, std::string defaultVal = "");
	void setProperty(std::string key, std::string val);

	int getProperty(std::string key, int defaultVal = 0);
	void setProperty(std::string key, int val);

private:
	bool hasConnection(CConnection* conn);

	CRootItem* getRev(const std::map<unsigned, CRootItem*>& map,
			          const unsigned rev,
			          const std::string& message) const throw(InvalidMsgException);

	// CTcpServer* m_tcp_server;
	std::string m_name;
	std::set<CConnection*> m_connections;

	std::map<unsigned, CRootItem*> m_mediaColRevs;
	std::map<unsigned, CRootItem*> m_playlistRevs;
	std::map<unsigned, CRootItem*> m_nextlistRevs;

    unsigned m_maxMediaColRev;
    unsigned m_maxPlaylistRev;
    unsigned m_maxNextlistRev;

    unsigned m_minMediaColRev;
    unsigned m_minPlaylistRev;
    unsigned m_minNextlistRev;

    unsigned m_playlistPos;

    std::string m_stateDBFilename;
    CStateDB* m_stateDB;

    // boost::asio::io_service& m_io_service;
    CMediaScannerCtrl* m_mediaScanner;

    CApp* m_app;
    std::string privatePropertyKey(std::string key);
};

} /* namespace muroa */
#endif /* CSESSION_H_ */
