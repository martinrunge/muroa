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
#include <log4cplus/loggingmacros.h>

#include <iostream>

using namespace std;
using namespace log4cplus;

namespace muroa {

CDnsSdBase::CDnsSdBase(boost::asio::io_service& io_service) : m_io_service(io_service),
		                                                      m_service_changed_handler(0),
		                                                      m_add_service_handler(0),
		                                                      m_rm_service_handler(0)
{
	m_app = CApp::getInstPtr();
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
	LOG4CPLUS_TRACE(Logger::getInstance("main"), "service added ..."  );
	if(m_service_changed_handler) {
		m_io_service.post( boost::bind(m_service_changed_handler) );
	}
	if(m_add_service_handler) {
		m_io_service.post( boost::bind(m_add_service_handler, newService) );
	}
}

int CDnsSdBase::removeService(std::string name)
{
	lock_guard<mutex> lk(m_mutex);
	int num = 0;
	ServDescPtr rmSrv;
	vector<ServDescPtr>::iterator it;
	for(it = m_services.begin(); it != m_services.end(); it++)
	{
		if( name.compare( (*it)->getServiceName() ) == 0 )
		{
			rmSrv = *it;
            vector<ServDescPtr>::iterator tmpit = it;
			m_services.erase(tmpit);
			--it;
			num++;
		}
	}

	if(num > 0) {
		LOG4CPLUS_TRACE(m_app->logger(), "service changed."  );
		if(m_service_changed_handler)
		{
			m_io_service.post(boost::bind(m_service_changed_handler));
		}
		if(m_rm_service_handler)
		{
			m_io_service.post(boost::bind(m_rm_service_handler, rmSrv));
		}
	}

	return num;
}

int CDnsSdBase::removeService(const CServiceDesc& rmSd )
{
	string serviceName = rmSd.getServiceName();
    int num = removeService(serviceName);
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


ServDescPtr CDnsSdBase::getServiceByName(string name)
{
	vector<ServDescPtr>::iterator it;
	for(it = m_services.begin(); it != m_services.end(); it++)
	{
		if( name.compare( (*it)->getServiceName() ) == 0 )
		{
			return *it;
		}
	}
	return ServDescPtr();
}

void CDnsSdBase::setServiceChangedHandler(service_changed_handler_t handler) {
	m_service_changed_handler = handler;
}

void CDnsSdBase::setServiceAddedHandler(add_service_handler_t handler) {
	m_add_service_handler = handler;
}

void CDnsSdBase::setServiceRemovedHandler(add_service_handler_t handler) {
	m_rm_service_handler = handler;
}


}
