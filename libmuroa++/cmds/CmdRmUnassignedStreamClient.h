/*
 * CmdAddUnassignedStreamClient.h
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#ifndef CMDRMUNASSIGNEDSTREAMCLIENT_H_
#define CMDRMUNASSIGNEDSTREAMCLIENT_H_

#include "Cmd.h"

namespace muroa {

class CmdRmUnassignedStreamClient: public muroa::Cmd {
public:
	CmdRmUnassignedStreamClient(std::string service_name);
	virtual ~CmdRmUnassignedStreamClient();

	std::string getServiceName() const {
		return m_service_name;
	}

private:
	std::string m_service_name;
};

} /* namespace muroa */
#endif /* CMDADDUNASSIGNEDSTREAMCLIENT_H_ */
