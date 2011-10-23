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
	// TODO Auto-generated constructor stub

}

CServiceDesc::CServiceDesc(string serviceName, string hostName, string domainName, int portNr, int interface, int protocol)
{
	m_serviceName = serviceName;
	m_hostName = hostName;
	m_domainName = domainName;
	m_portNr = portNr;

	m_interface = interface;
	m_protocol = protocol;
}


CServiceDesc::~CServiceDesc() {
	// TODO Auto-generated destructor stub
}

}
