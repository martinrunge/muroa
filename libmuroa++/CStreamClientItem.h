/***************************************************************************
 *
 *   CStreamClientItem.h
 *
 *   This file is part of libmuroa++                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CSTREAMCLIENTITEM_H_
#define CSTREAMCLIENTITEM_H_

#include "IContentItem.h"
#include "MuroaExceptions.h"

class CStreamClientItem: public IContentItem {
public:
	CStreamClientItem(CRootItem *root_item, CCategoryItem*  parent, std::string service_name, int posInParent = -1);
	CStreamClientItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent = -1) throw(ExMalformedPatch);
	virtual ~CStreamClientItem();

	bool operator==(const IContentItem& other);
	inline bool operator!=(const IContentItem& other){ return !operator==(other); };

	std::string serialize(bool asDiff = false);
	std::string getDomainName() const;
	void setDomainName(std::string domainName);
	bool isEnabled() const;
	void setEnabled(bool enabled);
	std::string getHostName() const;
	void setHostName(std::string hostName);
	int32_t getPort() const;
	void setPort(int32_t port);
	std::string getOwnerSessionName() const;
	void setOwnerSessionName(std::string ownerSessionName);
	std::string getServiceName() const;
//	void setServiceName(std::string serviceName);

	bool isAssignedToSession() { return m_owner_session_name.empty(); };

private:
	std::string m_service_name;
	std::string m_host_name;
	std::string m_domain_name;
	int32_t m_port;
	std::string m_owner_session_name;
	bool m_enabled;


	void assembleText();

	static const std::string DISABLED_STR;
	static const std::string ENABLED_STR;


};

#endif /* CSTREAMCLIENTITEM_H_ */
