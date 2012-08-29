/*
 * CSessionContainer.cpp
 *
 *  Created on: 12 Nov 2011
 *      Author: martin
 */

#include "CSessionContainer.h"
#include "muroaConstants.h"

#include "avahi/CDnsSdAvahi.h"

#include "CSession.h"
#include <boost/asio.hpp>
#include <algorithm>

#include "sessionEx.h"

namespace muroa {

using namespace std;
using namespace log4cplus;
using namespace muroa;

CSessionContainer* CSessionContainer::m_inst_ptr = 0;
std::mutex CSessionContainer::m_mutex;

struct RetrieveKey
{
    template <typename T>
    typename T::first_type operator()(T keyValuePair) const
    {
        return keyValuePair.first;
    }
};

CSessionContainer::CSessionContainer( CApp* app ) : m_tcp_server(0), m_dnssd(0), m_app(app), m_settings(app->settings()) {

}

void CSessionContainer::setup( boost::asio::io_service& io_service) {

	m_settings.setServiceName("Muroa Session Server");
	m_settings.setServiceType("_muroa._tcp");

	m_tcp_server = new CTcpServer(io_service, this, m_app, reinterpret_cast<factory_ptr_t>(&CConnection::create));

	m_sigPtr = CSignalHandler::create(io_service);
	m_sigPtr->start();

	vector<string> browselist;
	browselist.push_back("_muroa._tcp");
	browselist.push_back("_muroad._udp");

	m_dnssd = new CDnsSdAvahi(io_service, m_settings.serviceName(), m_settings.port(), m_settings.serviceType(), browselist);
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CSessionContainer::serviceChanged, this));
	m_dnssd->setServiceAddedHandler(boost::bind( &muroa::CSessionContainer::serviceAdded, this, _1));
	m_dnssd->setServiceRemovedHandler(boost::bind( &muroa::CSessionContainer::serviceRemoved, this, _1));

	CSession* createNewSession = new CSession(CREATE_NEW_SESSION, io_service, this);
	m_sessions.insert(pair<string, CSession*>(CREATE_NEW_SESSION, createNewSession));
}

CSessionContainer::~CSessionContainer() {
	if(m_dnssd != 0) {
		delete m_dnssd;
	}
}

void CSessionContainer::add(CTcpConnection* c) {
	CConnection* cp = static_cast<CConnection*>(c);
	m_sessionless_connections.insert(cp);
}

void CSessionContainer::remove(CTcpConnection* c) {
	CConnection* cp = static_cast<CConnection*>(c);
	m_sessionless_connections.erase(cp);
}

void CSessionContainer::removeAll() {
	  std::for_each(m_sessionless_connections.begin(), m_sessionless_connections.end(), boost::bind(&CConnection::stop, _1));
	  m_sessionless_connections.clear();
}

CSessionContainer *CSessionContainer::create(boost::asio::io_service & io_service, CApp *app) {
	lock_guard<mutex> lk(m_mutex);
	if( m_inst_ptr == 0) {
		m_inst_ptr = new CSessionContainer(app);
		m_inst_ptr->setup(io_service);
	}
	return m_inst_ptr;
}

std::vector<std::string> CSessionContainer::listSessions() {

	vector<string> sessionNames;

	// Retrieve all names
	transform(m_sessions.begin(), m_sessions.end(), back_inserter(sessionNames), RetrieveKey());

	return sessionNames;
}

void CSessionContainer::addSessionlessConnection(CConnection* ptr) {
	m_sessionless_connections.insert(ptr);
}

CSession* CSessionContainer::assignConnection(CConnection* ptr, std::string sessionName) {
	CSession* session;

	map<string, CSession*>::iterator it;
	it = m_sessions.find( sessionName );
	if(it == m_sessions.end()) {
		session = new CSession(sessionName, ptr->getIoService(), this);
		m_sessions.insert(pair<string, CSession*>(sessionName, session));
	}
	else {
		session = it->second;
	}

	session->addConnection(ptr);
	remove(ptr);
	return session;
}

void CSessionContainer::serviceChanged() {

}

void CSessionContainer::serviceAdded(ServDescPtr srvPtr) {
	string name = srvPtr->getServiceName();
	int num = 0;

	LOG4CPLUS_DEBUG(m_app->logger(), "service " << name << " appeared.");
	map<std::string, CSession*>::iterator it;
	for(it = m_sessions.begin(); it != m_sessions.end(); it++)
	{
		if(it->second->hasClient(name)) {
			it->second->enableClient(name);
			num++;
		}
	}
	if(num > 1) {
		LOG4CPLUS_ERROR(m_app->logger(), "more than on session claimed client '" << name << "'. A client can only be part of one session!");
	}

	if(num == 0) {
		LOG4CPLUS_DEBUG(m_app->logger(), "service " << name << " not part of any session.");
		m_unassignedClientNames.insert(name);
	}

}

void CSessionContainer::serviceRemoved(std::string name) {
	int num = 0;

	LOG4CPLUS_DEBUG(m_app->logger(), "service " << name << " disappeared.");
	map<std::string, CSession*>::iterator it;
	for(it = m_sessions.begin(); it != m_sessions.end(); it++)
	{
		if(it->second->hasClient(name)) {
			it->second->disableClient(name);
			num++;
		}
	}
	if(num > 1) {
		LOG4CPLUS_ERROR(m_app->logger(), "more than on session claimed client '" << name << "'. A client can only be part of one session!");
	}

	set<string>::iterator it2 = m_unassignedClientNames.find(name);
	if(it2 !=  m_unassignedClientNames.end())
	{
		LOG4CPLUS_DEBUG(m_app->logger(), "removing service " << name << " from list of clients not assigned to any session.");
		m_unassignedClientNames.erase(it2);
	}
}

ServDescPtr CSessionContainer::getServiceByName(std::string name) {
	m_dnssd->getServiceByName(name);
}



} /* namespace muroa */
