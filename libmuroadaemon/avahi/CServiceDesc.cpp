/*
 * CServiceDesc.cpp
 *
 *  Created on: 28 Mar 2010
 *      Author: martin
 */

#include "CServiceDesc.h"

using namespace std;

namespace muroa {



CServiceDesc::CServiceDesc() {
}

CServiceDesc::CServiceDesc(string serviceName, string hostName, string domainName, string serviceType, bip::address addr, int portNr, int interface, int protocol)
{
	m_serviceName = serviceName;
	m_hostName = hostName;
	m_domainName = domainName;
	m_serviceType = serviceType;
	m_address = addr;
	m_portNr = portNr;

	m_interface = interface;
	m_protocol = protocol;
}


CServiceDesc::~CServiceDesc() {
}

}
