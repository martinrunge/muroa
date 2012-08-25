/*
 * CSessionContainer.h
 *
 *  Created on: 12 Nov 2011
 *      Author: martin
 */

#ifndef CSESSIONCONTAINER_H_
#define CSESSIONCONTAINER_H_

#include "IConnectionManager.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <mutex>

#include "CTcpServer.h"
#include "CConnection.h"
#include "CSignalHandler.h"
#include "avahi/CDnsSdAvahi.h"

namespace muroa {

class CSession;
class CApp;
class CDnsSdAvahi;

class CSessionContainer : public IConnectionManager {

private:
	CSessionContainer( CApp* app );
	void setup(boost::asio::io_service& io_service);

public:
	virtual ~CSessionContainer();

	void add(CTcpConnection* c);
	void remove(CTcpConnection* c);
	void removeAll();

	static CSessionContainer* create( boost::asio::io_service& io_service, CApp* app );
	static CSessionContainer* getInstPtr( ) { return m_inst_ptr; };
	std::vector<std::string> listSessions();

	void addSessionlessConnection(CConnection* ptr);
	CSession* assignConnection(CConnection* ptr, std::string sessionName);

	void serviceChanged();
	void serviceAdded(ServDescPtr srvPtr);
	void serviceRemoved(std::string name);


private:
	static CSessionContainer* m_inst_ptr;
	static std::mutex m_mutex;

	CTcpServer* m_tcp_server;
    CDnsSdAvahi* m_dnssd;

	CApp* m_app;
	CSettings& m_settings;

	CSignalHandler* m_sigPtr;

	std::map<std::string, CSession*> m_sessions;
	std::set<CConnection*> m_sessionless_connections;

	std::set<std::string> m_unassignedClientNames;
};

} /* namespace muroa */
#endif /* CSESSIONCONTAINER_H_ */
