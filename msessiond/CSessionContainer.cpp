/*
 * CSessionContainer.cpp
 *
 *  Created on: 12 Nov 2011
 *      Author: martin
 */

#include "CSessionContainer.h"
#include "muroaConstants.h"

#include "cmds/CmdAddUnassignedStreamClient.h"
#include "cmds/CmdRmUnassignedStreamClient.h"

#include "avahi/CDnsSdAvahi.h"

#include <CTimeServiceCtrl.h>

#include <log4cplus/loggingmacros.h>

#include "CSession.h"
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>

#include "sessionEx.h"

namespace muroa {

using namespace std;
using namespace log4cplus;
using namespace muroa;
using boost::asio::ip::tcp;

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

	boost::asio::ip::tcp protocol = tcp::v4();
	if( m_settings.ipversion() == 6 ) {
		protocol = tcp::v6();
	}

	tcp::endpoint endp = tcp::endpoint(protocol, m_settings.getConfigVal("msessiond/port", 44555));

	m_tcp_server = new CTcpServer(io_service, this, endp, reinterpret_cast<factory_ptr_t>(&CConnection::create));

	const int port(endp.port());
	m_settings.setPersistentVal(string("msessiond/port"), port);

	m_sigPtr = CSignalHandler::create(io_service);
	m_sigPtr->start();

	if(m_app->settings().getConfigVal("enable_time_service", true)) {
		m_ts_ctrl = new CTimeServiceCtrl();
		int portNr = m_app->settings().getConfigVal("time_service_port", 44401);
		boost::asio::ip::address address;
		m_ts_ctrl->start(true, address, portNr);
	}

	vector<string> browselist;
	browselist.push_back("_muroa._tcp");
	browselist.push_back("_muroad._udp");

	m_dnssd = new CDnsSdAvahi(io_service, m_settings.serviceName(), port, m_settings.serviceType(), browselist);
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CSessionContainer::serviceChanged, this));
	m_dnssd->setServiceAddedHandler(boost::bind( &muroa::CSessionContainer::serviceAdded, this, _1));
	m_dnssd->setServiceRemovedHandler(boost::bind( &muroa::CSessionContainer::serviceRemoved, this, _1));

	CSession* createNewSession = new CSession(CREATE_NEW_SESSION, io_service, this, m_ts_ctrl->getCurrentServerPort());
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
		session = new CSession(sessionName, ptr->getIoService(), this, m_ts_ctrl->getCurrentServerPort());
		m_sessions.insert(pair<string, CSession*>(sessionName, session));

		// add all currently known services to the new session
		const std::vector<ServDescPtr> slist = m_dnssd->getServiceList();
	    std::vector<ServDescPtr>::const_iterator it;
	    for(it = slist.begin(); it != slist.end(); it++) {
	        if((*it)->getServiceType().compare("_muroad._udp") == 0) {
	            session->addClient((*it)->getServiceName());
	        }
	    }
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

	if(srvPtr->getServiceType().compare("_muroad._udp") == 0) {
		LOG4CPLUS_DEBUG(m_app->logger(), "service " << name << " appeared.");
		map<std::string, CSession*>::iterator it;
		for(it = m_sessions.begin(); it != m_sessions.end(); it++)
		{
		    if(it->second->getName().compare("createNewSession") != 0) {
		        // sesion named "createNewSession" is a dummy, don't serve it
		        it->second->addClient(name);
		    }
		}
	}
}

void CSessionContainer::serviceRemoved(ServDescPtr srvPtr) {
	int num = 0;

	if(srvPtr->getServiceType().compare("_muroad._udp") == 0) {
		LOG4CPLUS_DEBUG(m_app->logger(), "service " << srvPtr->getServiceName() << " disappeared.");
		map<std::string, CSession*>::iterator it;
		for(it = m_sessions.begin(); it != m_sessions.end(); it++)
		{
            if(it->second->getName().compare("createNewSession") != 0) {
                // sesion named "createNewSession" is a dummy, don't serve it
                it->second->rmClient(srvPtr->getServiceName());
            }
		}
	}
}

void CSessionContainer::serviceTaken(std::string seviceName, std::string ownerSessionsName) {
	LOG4CPLUS_DEBUG(m_app->logger(), "service " << seviceName << " was taken by session '" << ownerSessionsName << "'" );
	map<std::string, CSession*>::iterator it;
	for(it = m_sessions.begin(); it != m_sessions.end(); it++)
	{
        if(it->second->getName().compare("createNewSession") != 0) {
            // sesion named "createNewSession" is a dummy, don't serve it
            it->second->takeClient(seviceName, ownerSessionsName);
        }
	}
}


ServDescPtr CSessionContainer::getServiceByName(std::string name) {
	return m_dnssd->getServiceByName(name);
}

void CSessionContainer::toAll(Cmd* cmd)
{
	map<string, CSession*>::iterator it;
	for(it = m_sessions.begin(); it != m_sessions.end(); it++)
	{
		(*it).second->toAll(cmd, false);
	}
	delete cmd;
}


} /* namespace muroa */
