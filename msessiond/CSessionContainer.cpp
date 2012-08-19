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
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CApp::serviceChanged, m_app));

//	m_dnssd->addBrowseService("_muroa._tcp");
//	m_dnssd->addBrowseService("_muroad._udp");

	CSession* createNewSession = new CSession(CREATE_NEW_SESSION, io_service);
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
		session = new CSession(sessionName, ptr->getIoService());
		m_sessions.insert(pair<string, CSession*>(sessionName, session));
	}
	else {
		session = it->second;
	}

	session->addConnection(ptr);
	remove(ptr);
	return session;
}



} /* namespace muroa */
