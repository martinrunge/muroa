/*
 * CSession.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CSESSION_H_
#define CSESSION_H_

#include <vector>
#include <set>
#include <CConnection.h>

namespace muroa {

class CCmdBase;
class CTcpServer;

class CSession {
public:
	CSession();
	virtual ~CSession();

	void toAll( CCmdBase* cmd );

private:

	std::set<CTcpConnection::pointer> getConnections();

	bool hasConnection(CConnection::pointer conn);

	CTcpServer* m_tcp_server;
};

} /* namespace muroa */
#endif /* CSESSION_H_ */
