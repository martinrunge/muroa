/*
 * CmdRmStreamClient.cpp
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#include "CmdRmStreamClient.h"

namespace muroa {

CmdRmStreamClient::CmdRmStreamClient(std::string service_name) : Cmd(Cmd::RM_CLIENT_FROM_SESSION),
		                                                         m_service_name(service_name)
{
}

CmdRmStreamClient::~CmdRmStreamClient()
{
}

} /* namespace muroa */
