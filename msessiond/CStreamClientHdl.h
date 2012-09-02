/*
 * CStreamClientHdl.h
 *
 *  Created on: 2 Sep 2012
 *      Author: martin
 */

#ifndef CSTREAMCLIENTHDL_H_
#define CSTREAMCLIENTHDL_H_

#include <string>

class CRootItem;

namespace muroa {

class CSession;

/**
*  A session remembers its streaming clients even if they disappear. They are marked as disabled then.
*  If a Streaming client appeares again, it is added to this session again and enabled.
*  If a client appears that is not part of any session (disabled there), it gets added to availableClients.
*/
class CStreamClientHdl {
public:
	CStreamClientHdl(CSession* session);
	virtual ~CStreamClientHdl();

	CRootItem* buildStateRevClientChange(CRootItem* curState,
	                                     std::string addClient,
	                                     std::string rmClient,
	                                     std::string enableClient,
	                                     std::string disableClient);

	std::pair<CRootItem*, std::string> addClientStateDiff(const CRootItem* const curState, const std::string& addClient);
	std::pair<CRootItem*, std::string> rmClientStateDiff(const CRootItem* const curState, const std::string& rmClient);

	int hasClient(std::string name,std::string category=std::string("/OwnStreamClients"));

private:
	CSession* m_session;
};

} /* namespace muroa */
#endif /* CSTREAMCLIENTHDL_H_ */
