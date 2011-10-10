/*
 * CDnsSdBase.cpp
 *
 *  Created on: 25 Mar 2010
 *      Author: martin
 */

#include "CDnsSdBase.h"

CDnsSdBase::CDnsSdBase() : m_notify(false){
}

CDnsSdBase::~CDnsSdBase() {
}


void CDnsSdBase::addService(CServiceDesc* newService)
{
	m_serviceList.append(newService);
	if( m_notify ) {
		if( newService->getServiceName().compare(m_notify_service ) == 0 &&
			newService->getHostName().compare(m_notify_host) == 0 &&
			newService->getDomainName().compare(m_notify_domain) == 0 )
		{
			emit notifyService( *newService );
		}
	}
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

int CDnsSdBase::removeService(const CServiceDesc& rmSd )
{
	int num = 0;
	for(int i=0; i < m_serviceList.size(); i++)
	{
		if( *m_serviceList.at(i) == rmSd )
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


CServiceDesc* CDnsSdBase::getService(QString name, int which)
{
	int num = 0;
	for(int i=0; i < m_serviceList.size(); i++)
	{
		if( name.compare( m_serviceList.at(i)->getServiceName() ) == 0 )
		{
			if(num == which)
			{
				return m_serviceList.at(i);
			}
			num++;
		}
	}
	return 0;
}

void CDnsSdBase::notifyOn(QString service, QString host, QString domain)
{
	if( service.isEmpty() || host.isEmpty() || domain.isEmpty() ) {
		m_notify = false;
	}
	else {
		m_notify = true;
		m_notify_service = service;
		m_notify_host = host;
		m_notify_domain = domain;
	}
}

