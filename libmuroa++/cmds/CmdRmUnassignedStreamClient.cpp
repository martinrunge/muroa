/*
 * CmdRmUnassignedStreamClient.cpp
 *
 *  Created on: 29 Aug 2012
 *      Author: martin
 */

#include "CmdRmUnassignedStreamClient.h"

namespace muroa {

CmdRmUnassignedStreamClient::CmdRmUnassignedStreamClient(std::string service_name) : Cmd(Cmd::RM_UNASSIGNED_CLIENT),
		                                                                             m_service_name(service_name)
{
}

CmdRmUnassignedStreamClient::~CmdRmUnassignedStreamClient()
{
}

} /* namespace muroa */
