/*
 * CDnsSdBase.cpp
 *
 *  Created on: 25 Mar 2010
 *      Author: martin
 */

#include <mutex>
#include <boost/bind.hpp>
#include "CDnsSdBase.h"

#include <iostream>

using namespace std;

namespace muroa {

CDnsSdBase::CDnsSdBase(boost::asio::io_service& io_service) : m_notify(false) ,
		                                                      m_io_service(io_service),
		                                                      m_notify_handler(0),
		                                                      m_service_changed_handler(0)
{
}

CDnsSdBase::~CDnsSdBase() {
}

vector<ServDescPtr> CDnsSdBase::getServiceList() const
{
    return m_services;
}

void CDnsSdBase::setServiceList(vector<ServDescPtr> services)
{
	lock_guard<mutex> lk(m_mutex);
    m_services = services;
}

void CDnsSdBase::addService(ServDescPtr newService)
{
	// lock_guard<mutex> lk(m_mutex);
	m_services.push_back(newService);
	if( m_notify ) {
		if( newService->getServiceName().compare(m_notify_service ) == 0 &&
			newService->getHostName().compare(m_notify_host) == 0 &&
			newService->getDomainName().compare(m_notify_domain) == 0 )
		{
			if(m_notify_handler) {
				m_io_service.post( boost::bind(m_notify_handler, newService) );
			}
		}
	}
	if(m_service_changed_handler) {
		cerr << "addService: posting to io_service ..." << endl;
		m_io_service.post( boost::bind(m_service_changed_handler) );
	}
}

int CDnsSdBase::removeService(std::string name)
{
	lock_guard<mutex> lk(m_mutex);
	int num = 0;
	vector<ServDescPtr>::iterator it;
	for(it = m_services.begin(); it != m_services.end(); it++)
	{
		if( name.compare( (*it)->getServiceName() ) == 0 )
		{
			m_services.erase(it);
			num++;
		}
	}
	return num;
}

int CDnsSdBase::removeService(const CServiceDesc& rmSd )
{
	lock_guard<mutex> lk(m_mutex);
	int num = 0;
	vector<ServDescPtr>::iterator it;
	for(it = m_services.begin(); it != m_services.end(); it++)
	{
		if( **it == rmSd )
		{
			m_services.erase(it);
			num++;
		}
	}

	if(num > 0) {
        m_io_service.post(boost::bind(m_service_changed_handler));
	}

	return num;
}

int CDnsSdBase::hasService(string name)
{
	int num = 0;
	vector<ServDescPtr>::iterator it;
	for(it = m_services.begin(); it != m_services.end(); it++)
	{
		if( name.compare( (*it)->getServiceName() ) == 0 )
		{
			num++;
		}
	}
	return num;
}


ServDescPtr CDnsSdBase::getService(string name, int which)
{
	int num = 0;
	vector<ServDescPtr>::iterator it;
	for(it = m_services.begin(); it != m_services.end(); it++)
	{
		if( name.compare( (*it)->getServiceName() ) == 0 )
		{
			if(num == which)
			{
				return *it;
			}
			num++;
		}
	}
	return ServDescPtr();
}

void CDnsSdBase::notifyOn(string service, string host, string domain)
{
	lock_guard<mutex> lk(m_mutex);
	if( service.empty() || host.empty() || domain.empty() ) {
		m_notify = false;
	}
	else {
		m_notify = true;
		m_notify_service = service;
		m_notify_host = host;
		m_notify_domain = domain;
	}
}

void CDnsSdBase::setNotifyHandler(notify_handler_t handler) {
	m_notify_handler = handler;
}

void CDnsSdBase::setServiceChangedHandler(service_changed_handler_t handler) {
	m_service_changed_handler = handler;
}


}
