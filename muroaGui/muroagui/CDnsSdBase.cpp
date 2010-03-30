/*
 * CDnsSdBase.cpp
 *
 *  Created on: 25 Mar 2010
 *      Author: martin
 */

#include "CDnsSdBase.h"
#include "CServiceDesc.h"

CDnsSdBase::CDnsSdBase() {
}

CDnsSdBase::~CDnsSdBase() {
}


void CDnsSdBase::addService(CServiceDesc* newService)
{
	m_serviceList.append(newService);
}

int CDnsSdBase::removeService(QString name)
{
	int num = 0;
	for(int i=0; i < m_serviceList.size(); i++)
	{
		if( name.compare( m_serviceList.at(i)->getServiceName() ) == 0 )
		{
			CServiceDesc* serv = m_serviceList.takeAt(i);
			delete serv;
			num++;
		}
	}
	return num;
}

int CDnsSdBase::hasService(QString name)
{
	int num = 0;
	for(int i=0; i < m_serviceList.size(); i++)
	{
		if( name.compare( m_serviceList.at(i)->getServiceName() ) == 0 )
		{
			num++;
		}
	}
	return num;
}
