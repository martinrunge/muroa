/*
 * CmdAddUnassignedStreamClient.cpp
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#include "CmdAddUnassignedStreamClient.h"

namespace muroa {

CmdAddUnassignedStreamClient::CmdAddUnassignedStreamClient(std::string service_name,
		                                                   std::string host,
		                                                   int port) : Cmd(Cmd::ADD_UNASSIGNED_CLIENT),
		                                                               m_service_name(service_name),
		                                                               m_host(host),
		                                                               m_port(port)
{

}

CmdAddUnassignedStreamClient::~CmdAddUnassignedStreamClient()
{
}

} /* namespace muroa */
