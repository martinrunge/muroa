/*
 * CmdAddUnassignedStreamClient.h
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#ifndef CMDADDUNASSIGNEDSTREAMCLIENT_H_
#define CMDADDUNASSIGNEDSTREAMCLIENT_H_

#include "Cmd.h"

namespace muroa {

class CmdAddUnassignedStreamClient: public muroa::Cmd {
public:
	CmdAddUnassignedStreamClient(std::string service_name, std::string host = "", int port = 0);
	virtual ~CmdAddUnassignedStreamClient();

	std::string getHost() const {
		return m_host;
	}

	int getPort() const {
		return m_port;
	}

	std::string getServiceName() const {
		return m_service_name;
	}

private:
	std::string m_service_name;
	std::string m_host;
	int m_port;
};

} /* namespace muroa */
#endif /* CMDADDUNASSIGNEDSTREAMCLIENT_H_ */
