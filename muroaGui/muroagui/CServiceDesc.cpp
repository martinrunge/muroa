/*
 * CServiceDesc.cpp
 *
 *  Created on: 28 Mar 2010
 *      Author: martin
 */

#include "CServiceDesc.h"

CServiceDesc::CServiceDesc() {
	// TODO Auto-generated constructor stub

}

CServiceDesc::CServiceDesc(QString serviceName, QString hostName, QString domainName, int portNr, int interface, int protocol)
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
