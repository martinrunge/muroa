/*
 * CmdAddStreamClient.cpp
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#include "CmdAddStreamClient.h"

namespace muroa {

CmdAddStreamClient::CmdAddStreamClient(std::string service_name,
		                               std::string host,
		                               int port) : Cmd(Cmd::ADD_CLIENT_TO_SESSION),
		                                           m_service_name(service_name),
		                                           m_host(host),
		                                           m_port(port)
{
}

CmdAddStreamClient::~CmdAddStreamClient()
{
}

} /* namespace muroa */
