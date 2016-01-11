/*
 * CStreamClientDicovery.cpp
 *
 *  Created on: 10.01.2016
 *      Author: martin
 */

#include "CStreamClientDiscovery.h"
#include <avahi/CDnsSdAvahi.h>

#include <vector>

namespace muroa {

using namespace std;

CStreamClientDiscovery::CStreamClientDiscovery(boost::asio::io_service& io_service) {

	vector<string> browselist;
	browselist.push_back("_muroad._tcp");

	m_dnssd = new CDnsSdAvahi(io_service, "", 0, "", browselist);
	m_dnssd->setServiceChangedHandler(boost::bind( &muroa::CStreamClientDiscovery::onClientChanged, this));
	m_dnssd->setServiceAddedHandler(boost::bind( &muroa::CStreamClientDiscovery::onClientAppeared, this, _1));
	m_dnssd->setServiceRemovedHandler(boost::bind( &muroa::CStreamClientDiscovery::onClientDisappeared, this, _1));

}

CStreamClientDiscovery::~CStreamClientDiscovery() {
	delete m_dnssd;
}

void CStreamClientDiscovery::onClientAppeared(ServDescPtr srvPtr) {
}

void CStreamClientDiscovery::onClientDisappeared(ServDescPtr srvPtr) {
}

void CStreamClientDiscovery::onClientChanged() {
}

} /* namespace muroa */
