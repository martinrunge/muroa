/*
 * CDnsSdBase.cpp
 *
 *  Created on: 25 Mar 2010
 *      Author: martin
 */

#include <mutex>
#include <boost/bind.hpp>
#include "CDnsSdBase.h"

#include <log4cplus/logger.h>

#include <iostream>

using namespace std;
using namespace log4cplus;

namespace muroa {

CDnsSdBase::CDnsSdBase(boost::asio::io_service& io_service) : m_io_service(io_service),
		                                                      m_service_changed_handler(0)
{
	CApp* m_app = CApp::getInstPtr();
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
	if(m_service_changed_handler) {
		LOG4CPLUS_TRACE(Logger::getInstance("main"), "service added ..."  );
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
		LOG4CPLUS_TRACE(m_app->logger(), "service changed."  );
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

void CDnsSdBase::setServiceChangedHandler(service_changed_handler_t handler) {
	m_service_changed_handler = handler;
}


}
