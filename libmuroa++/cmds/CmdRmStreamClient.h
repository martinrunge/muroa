/*
 * CmdRmStreamClient.h
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#ifndef CMDRMSTREAMCLIENT_H_
#define CMDRMSTREAMCLIENT_H_

#include "Cmd.h"

namespace muroa {

class CmdRmStreamClient: public muroa::Cmd {
public:
	CmdRmStreamClient(std::string service_name);
	virtual ~CmdRmStreamClient();

	std::string getServiceName() const {
		return m_service_name;
	}

private:
	std::string m_service_name;
};

} /* namespace muroa */
#endif /* CMDADDUNASSIGNEDSTREAMCLIENT_H_ */
