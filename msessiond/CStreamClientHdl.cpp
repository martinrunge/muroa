/*
 * CStreamClientHdl.cpp
 *
 *  Created on: 2 Sep 2012
 *      Author: martin
 */

#include "CStreamClientHdl.h"
#include "CSession.h"
#include "CRootItem.h"
#include "CStreamClientItem.h"

#include <utility>

using namespace std;
using namespace muroa;

namespace muroa {

CStreamClientHdl::CStreamClientHdl(CSession* session) : m_session(session)
{

}

CStreamClientHdl::~CStreamClientHdl()
{

}

CRootItem* CStreamClientHdl::buildStateRevClientChange(CRootItem* curState,
                                                       string addClient,
                                                       string rmClient,
                                                       string enableClient,
                                                       string disableClient)
{
	CRootItem* newState = new CRootItem(*curState);

	return newState;
}

/**
 * \brief create a diff string representing an added streaming client
 *        As seen by this session.
 *
 *  This method gets called for all sessions when a streaming client appeares vid DNS-SD (Avahi)
 *  If the streaming client is part of a session, it is already known there, but disabled. It will be enabled.
 *	For all sessions this client is not part of, it gets available in the "availStreamingClients" section.
 *
 *	\return std::pair with the pointer to a new RootItem containing the new state and the diff between the current state and the new state as string.
 */
std::pair<CRootItem*, std::string> CStreamClientHdl::addClientStateDiff(const CRootItem* const curState, const std::string& addClient)
{
	ostringstream oss;

	CRootItem* newState = new CRootItem(*curState);

	int pos = hasClient(addClient);
	if(pos != -1) {
		// streaming client 'addClient' is assigned to this session, as it just appeared, enable it.
		CCategoryItem* base = newState->getCategoryPtr("/OwnStreamClients");
		IContentItem *tmp = base->getContentItem(pos);
		CStreamClientItem *sci = reinterpret_cast<CStreamClientItem*>(tmp);

		if(sci->isEnabled()) {
			oss << "@@ -" << pos + 1 << ",1 +" << pos + 1 << ",1 @@" << endl;
			oss << "-" << base->getPath() << "\t" << sci->serialize();
			sci->setEnabled(true);
			oss << "+" << base->getPath() << "\t" << sci->serialize();
		}
	}
	else {
		// streaming client is not assigned to this session -> mark it as available
		int pos = hasClient(addClient,"/AvailableStreamClients");
		if(pos != -1) {
			// streaming client is already listed, make sure entry is up to date.
			//TODO: check if streaming client's entry is correct, correct it if not.
		}
		else {
			int num_avail_clients;
			CCategoryItem* base = newState->getCategoryPtr("/AvailableStreamClients");
			if(base == 0) {
				base = newState->mkPath("/AvailableStreamClients");
				num_avail_clients = 0;
			}
			else {
				num_avail_clients = base->getNumContentItems();
			}

			ServDescPtr srvPtr = m_session->getServiceByName(addClient);
			assert(srvPtr != NULL);

			CStreamClientItem *sci = new CStreamClientItem(newState, base, addClient);
			sci->setHostName(srvPtr->getHostName());
			sci->setDomainName(srvPtr->getDomainName());
			sci->setPort(srvPtr->getPortNr());
			sci->setOwnerSessionName(m_session->getName());
			sci->setEnabled(false);

			oss << "@@ -0,0 +" << num_avail_clients + 1 << ",1 @@" << endl;
			oss << "+" << base->getPath() << "\t" << sci->serialize();
		}
	}
	return make_pair( newState, oss.str());
}

std::pair<CRootItem*, std::string> CStreamClientHdl::rmClientStateDiff(const CRootItem* const curState, const std::string& rmClient)
{
	ostringstream oss;
	CRootItem* newState = new CRootItem(*curState);

	return make_pair(newState, oss.str());
}

/**
 *  \brief  Check if a streaming client is assigned to this session
 *
 *  if yes, return its position in the "ownClients" category of the session's state
 *
 *  \return position in the "ownClients" category of the session's state if streaming client is assigned to this session, -1 if not.
 */
int CStreamClientHdl::hasClient(string name, string category)
{
	CRootItem* curState = m_session->getSessionState();
	CCategoryItem* base = curState->getCategoryPtr(category);

	if(base == NULL) {
		return -1;
	}

	int num_own_clients = base->getNumContentItems();
	for(int i=0; i < num_own_clients; i++ )
	{
		IContentItem *tmp = base->getContentItem(i);
		CStreamClientItem *sci = reinterpret_cast<CStreamClientItem*>(tmp);

		if(sci->getServiceName().compare(name) == 0)
		{
			return i;
		}
	}

	return -1;
}


} /* namespace muroa */
