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
 *	\return std::pair with the pointer to a new RootItem containing the new state and the diff between
 *	        the current state and the new state as string. Null pointer and empty string if nothing changed.
 */
std::pair<CRootItem*, std::string> CStreamClientHdl::addClientStateDiff(const CRootItem* const curState, const std::string& addClient)
{
	ostringstream oss;
	int num_clients;
	CRootItem* newState = new CRootItem(*curState);

	ServDescPtr srvPtr = m_session->getServiceByName(addClient);
	assert(srvPtr != NULL);

	CCategoryItem* base = newState->getCategoryPtr("/RenderClients");
	if(base == 0) {
		base = newState->mkPath("/RenderClients");
		num_clients = 0;
	}
	else {
		num_clients = base->getNumContentItems();
	}

	// create new client, but do NOT set its parent pointer so it will not be added to parent as child.
	// Add it as child of "base" if it is new, only.
    CStreamClientItem *newsci = new CStreamClientItem(newState, 0, addClient);
    newsci->setHostName(srvPtr->getHostName());
    newsci->setDomainName(srvPtr->getDomainName());
    newsci->setPort(srvPtr->getPortNr());
    newsci->setEnabled(true);


	pair<int,int> pospair = hasClient(addClient);
	int pos = pospair.first;
	int insertbefore = pospair.second;
	if(pos == -1) {
		// streaming client is not known -> append it as available
		oss << "@@ -" << insertbefore << ",0 +" << insertbefore + 1 << ",1 @@" << endl;
		oss << "+" << base->getPath() << "\t" << newsci->serialize();

		base->addChild(newsci, insertbefore);
	}
	else
	{
		// render client 'addClient' is already known, as it just appeared, enable it.
		// do not touch owner session'n name
		IContentItem *tmp = base->getContentItem(pos);
		CStreamClientItem *sci = reinterpret_cast<CStreamClientItem*>(tmp);

		newsci->setOwnerSessionName(sci->getOwnerSessionName());

		if(*sci != *newsci) {
			oss << "@@ -" << pos + 1 << ",1 +" << pos + 1 << ",1 @@" << endl;
			oss << "-" << base->getPath() << "\t" << sci->serialize();
			oss << "+" << base->getPath() << "\t" << newsci->serialize();
			base->delContentItem(pos);
			base->addChild(newsci, pos);
		}
		else
		{
			delete newState;
			return make_pair((CRootItem*)0, string());
		}
	}
	return make_pair( newState, oss.str());
}

std::pair<CRootItem*, std::string> CStreamClientHdl::rmClientStateDiff(const CRootItem* const curState, const std::string& rmClient)
{

    pair<int,int> pospair = hasClient(rmClient);
    int pos = pospair.first;
    int insertbefore = pospair.second;
	if(pos == -1) {
		// streaming client is not known -> don't care if it disappeared
		return make_pair((CRootItem*)0, string());
	}
	else {
		// render client 'rmClient' is already known, as it just disappeared, disable it.
		// do not touch owner session'n name
		ostringstream oss;
		CRootItem* newState = new CRootItem(*curState);

		/// TODO get service by name from session state! m_session->getServiceByName() gets it
		//   from DnsSDBase where it has already been deleted by now.

		CCategoryItem* base = newState->getCategoryPtr("/RenderClients");
		if(base == 0) {
			base = newState->mkPath("/RenderClients");
		}

        IContentItem* ci = base->getContentItem(pos);
        assert( ci->type() == CItemType::E_STREAM_CLIENT);
        CStreamClientItem *rmsci = reinterpret_cast<CStreamClientItem*>(ci);

        if( isOwnClient(rmsci) ) {
            if( rmsci->isEnabled() ) {
                // it was enabled, disable it
                oss << "@@ -" << pos + 1 << ",1 +" << pos + 1 << ",1 @@" << endl;
                oss << "-" << base->getPath() << "\t" << rmsci->serialize();
                rmsci->setEnabled(false);
                oss << "+" << base->getPath() << "\t" << rmsci->serialize();
            }
            else {
                // it is our own client, but it was already disbled -> do nothing
                // it was already disabled -> do nothing
                delete newState;
                return make_pair((CRootItem*)0, string());
            }
        }
        else {
            // client is known and disappeared now. As it is not owned by this session, just remove it.
            oss << "@@ -" << pos + 1 << ",1 +" << pos + 1 << ",0 @@" << endl;
            oss << "-" << base->getPath() << "\t" << rmsci->serialize();
            base->delContentItem(pos);
        }
        return make_pair(newState, oss.str());
	}
}

/**
 *  \brief  Take ownership of a renderclient
 *
 *  Ownership of a renderclient was taken by session 'ownerSessionsName'. If it
 *  belonged to this session before, relase ownershio. Fill in
 *  'ownerSessionName' as owner of render client named 'name'.
 *
 *  \return as a pair: both pointer to sessionState CRootItem and the diff as string
 */
std::pair<CRootItem*, std::string> CStreamClientHdl::takeClientStateDiff(const CRootItem* const curState,
		                                                                 const std::string& name,
		                                                                 const std::string& ownerSessionsName)
{
	ostringstream oss;
	CRootItem* newState = new CRootItem(*curState);

	ServDescPtr srvPtr = m_session->getServiceByName(name);
	assert(srvPtr != NULL);

	CCategoryItem* base = newState->getCategoryPtr("/RenderClients");
	if(base == 0) {
		base = newState->mkPath("/RenderClients");
	}

	CStreamClientItem *newsci = new CStreamClientItem(newState, base, name);
	newsci->setHostName(srvPtr->getHostName());
	newsci->setDomainName(srvPtr->getDomainName());
	newsci->setPort(srvPtr->getPortNr());
	newsci->setOwnerSessionName(ownerSessionsName);

    pair<int,int> pospair = hasClient(name);
    int pos = pospair.first;
    int insertbefore = pospair.second;
	if(pos == -1) {
		// streaming client is not known -> append it as available
		int num_clients;

		CCategoryItem* base = newState->getCategoryPtr("/RenderClients");
		if(base == 0) {
			base = newState->mkPath("/RenderClients");
			num_clients = 0;
		}
		else {
			num_clients = base->getNumContentItems();
		}

		oss << "@@ -0,0 +" << num_clients + 1 << ",1 @@" << endl;
		oss << "+" << base->getPath() << "\t" << newsci->serialize();
		base->addChild(newsci);
	}
	else
	{
		// render client 'name' is already known, ownership was taken by 'ownerSessionsName'.
		// do not touch enabled/disabled state
		IContentItem *tmp = base->getContentItem(pos);
		CStreamClientItem *sci = reinterpret_cast<CStreamClientItem*>(tmp);

		if(*sci != *newsci) {
			oss << "@@ -" << pos << ",1 +" << pos << ",1 @@" << endl;
			oss << "-" << base->getPath() << "\t" << sci->serialize();
			oss << "+" << base->getPath() << "\t" << newsci->serialize();
			sci->setOwnerSessionName(ownerSessionsName);
		}
		else
		{
			delete newState;
			return make_pair((CRootItem*)0, string());
		}
	}
	return make_pair( newState, oss.str());
}


/**
 *  \brief  Check if a streaming client is assigned to this session
 *
 *  if yes, return its position in the "RenderClients" category of the session's state
 *
 *  \return position in the "RenderClients" category of the session's state if
 *          render client is assigned to this session, -1 if not.
 */
int CStreamClientHdl::isOwnClient(string name, string category)
{
	CRootItem* curState = m_session->getSessionState();
	CCategoryItem* base = curState->getCategoryPtr(category);

	if(base == NULL) {
		return -1;
	}

	int num_clients = base->getNumContentItems();
	for(int i=0; i < num_clients; i++ )
	{
		IContentItem *tmp = base->getContentItem(i);
		CStreamClientItem *sci = reinterpret_cast<CStreamClientItem*>(tmp);

		if(sci->getServiceName().compare(name) == 0 ) {
		    if( isOwnClient(sci) ) {
		        return i;
		    }
		}
	}

	return -1;
}

/**
 *  \brief  Check if a render client is owned by this session
 *
 *  \return true if sci is owned by this session, false if not.
 */
bool CStreamClientHdl::isOwnClient(CStreamClientItem* sci)
{
    if(m_session->getName().compare( sci->getOwnerSessionName()) == 0 ) {
        return true;
    }
    else {
        return false;
    }
}


/**
 *  \brief  Check if a render client is known
 *
 *  if yes, return its position in the "RenderClients" category of the session's state
 *
 *  \return pair of ints: first: position in the "RenderClients" category of the session's state if
 *          render client is already known, -1 if not.
 *          second: if not found the insert position, -1 if found.
 *          on error, both are -1.
 */
pair<int,int> CStreamClientHdl::hasClient(string name, string category)
{
	CRootItem* curState = m_session->getSessionState();
	CCategoryItem* base = curState->getCategoryPtr(category);
	int foundpos = -1;
	int insertpos = 0;

	if(base == NULL) {
		return make_pair(-1, -1);
	}

	int num_clients = base->getNumContentItems();
	for(int i=0; i < num_clients; i++ )
	{
		IContentItem *tmp = base->getContentItem(i);
		CStreamClientItem *sci = reinterpret_cast<CStreamClientItem*>(tmp);

		string serviceName = sci->getServiceName();
		int cmpres = name.compare(serviceName);
		if(cmpres == 0)
		{
			return make_pair(i, -1);
		}
		if(cmpres > 0) {
		    insertpos = i + 1;
		}
	}

	return make_pair(-1, insertpos);
}

} /* namespace muroa */
