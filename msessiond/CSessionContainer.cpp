/*
 * CSessionContainer.cpp
 *
 *  Created on: 12 Nov 2011
 *      Author: martin
 */

#include "CSessionContainer.h"
#include "muroaConstants.h"

#include "CSession.h"
#include <boost/asio.hpp>
#include <algorithm>



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

CSessionContainer::CSessionContainer( CApp* app ) : m_tcp_server(0), m_app(app) {

}

void CSessionContainer::setup( boost::asio::io_service& io_service) {

	m_tcp_server = new CTcpServer(io_service, m_app, reinterpret_cast<factory_ptr_t>(&CConnection::create));

	m_sigPtr = CSignalHandler::create(io_service);
	m_sigPtr->start();

	CSession* createNewSession = new CSession(CREATE_NEW_SESSION);
	m_sessions.insert(pair<string, CSession*>(CREATE_NEW_SESSION, createNewSession));
}

CSessionContainer::~CSessionContainer() {

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

void CSessionContainer::addSessionlessConnection(CConnection::pointer ptr) {
	m_sessionless_connections.insert(ptr);
}

CSession* CSessionContainer::assignConnection(CConnection::pointer ptr, std::string sessionName) {
	CSession* session;

	map<string, CSession*>::iterator it;
	it = m_sessions.find( sessionName );
	if(it == m_sessions.end()) {
		session = new CSession(sessionName);
		m_sessions.insert(pair<string, CSession*>(sessionName, session));
	}
	else {
		session = it->second;
	}

	return session;
}



} /* namespace muroa */
