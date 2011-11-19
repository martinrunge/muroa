/*
 * CSession.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CSESSION_H_
#define CSESSION_H_

#include <string>
#include <vector>
#include <set>
#include <CConnection.h>

namespace muroa {

class CCmdBase;
class CTcpServer;

class CSession {
public:
	CSession(std::string name);
	virtual ~CSession();

	std::string getName() { return m_name; };

	void addConnection(CConnection::pointer ptr);
	void removeConnection(CConnection::pointer ptr);

	void toAll( CCmdBase* cmd );

private:
	bool hasConnection(CConnection::pointer conn);

	// CTcpServer* m_tcp_server;
	std::string m_name;
	std::set<CConnection::pointer> m_connections;
};

} /* namespace muroa */
#endif /* CSESSION_H_ */
