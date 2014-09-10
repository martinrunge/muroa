/*
 * CStreamClientHdl.h
 *
 *  Created on: 2 Sep 2012
 *      Author: martin
 */

#ifndef CSTREAMCLIENTHDL_H_
#define CSTREAMCLIENTHDL_H_

#include <string>
#include "IItemModel.h"

class CStream;

namespace muroa {

class CSession;
class CRootItem;
class CStreamClientItem;

/**
*  A session remembers its streaming clients even if they disappear. They are marked as disabled then.
*  If a Streaming client appeares again, it is added to this session again and enabled.
*  If a client appears that is not part of any session (disabled there), it gets added to availableClients.
*/
class CStreamClientHdl : public IItemModel {
public:
	CStreamClientHdl(CSession* session, CStream* stream);
	virtual ~CStreamClientHdl();

	CRootItem* buildStateRevClientChange(CRootItem* curState,
	                                     std::string addClient,
	                                     std::string rmClient,
	                                     std::string enableClient,
	                                     std::string disableClient);

	std::pair<CRootItem*, std::string> addClientStateDiff(const CRootItem* const curState, const std::string& addClient);
	std::pair<CRootItem*, std::string> rmClientStateDiff(const CRootItem* const curState, const std::string& rmClient);
	std::pair<CRootItem*, std::string> takeClientStateDiff(const CRootItem* const curState, const std::string& name, const std::string& ownerSessionsName);

	int isOwnClient(std::string name,std::string category=std::string("/RenderClients"));
    bool isOwnClient(CStreamClientItem* sci);
	std::pair<int,int> hasClient(std::string name,std::string category=std::string("/RenderClients"));

	// inherited from IItemModel
	bool beginInsertItems(const int pos, const int count, const CCategoryItem* parent);
	bool endInsertItems(void);
	bool beginRemoveItems(const int pos, const int count, const CCategoryItem* parent);
	bool endRemoveItems(void);

protected:
	// inherited from IItemModel
    void reset();


private:
	CSession* m_session;
	CStream* m_stream;

	const CCategoryItem* m_inserting_to_parent;
	int m_insert_pos;
	int m_insert_count;
	const CCategoryItem* m_removing_from_parent;
	int m_remove_pos;
	int m_remove_count;

	void assertNoActiveTransaction();
};

} /* namespace muroa */
#endif /* CSTREAMCLIENTHDL_H_ */
