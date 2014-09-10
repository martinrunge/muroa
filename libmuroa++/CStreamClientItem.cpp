/***************************************************************************
 *
 *   CStreamClientItem.cpp
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

#include "CStreamClientItem.h"
#include "CRootItem.h"
#include "CCategoryItem.h"
#include "CUtils.h"
#include <sstream>
#include <cassert>

using namespace std;
namespace muroa {

const string CStreamClientItem::DISABLED_STR("disabled");
const string CStreamClientItem::ENABLED_STR("enabled");

CStreamClientItem::CStreamClientItem(const CStreamClientItem& other, CRootItem* const root_item, CCategoryItem*  const parent) :
                   IContentItem( root_item, parent, CItemType::E_STREAM_CLIENT)
{
    m_domain_name = other.m_domain_name;
    m_enabled = other.m_enabled;
    m_hash = other.m_hash;
    m_host_name = other.m_host_name;
    m_name = other.m_name;
    m_owner_session_name = other.m_owner_session_name;
    m_port = other.m_port;
    m_service_name = other.m_service_name;
    m_text = other.m_text;

    m_root_item->setContentPtr(CItemType(CItemType::E_STREAM_CLIENT), this, m_hash );
}

CStreamClientItem::CStreamClientItem(CRootItem *root_item, CCategoryItem*  parent, string service_name, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_STREAM_CLIENT ),
               m_service_name(service_name)
{
	m_hash = hash(m_service_name);
	assembleText();
	m_root_item->setContentPtr(CItemType(CItemType::E_STREAM_CLIENT), this, m_hash );
	if(parent != 0) {
		parent->addChild(this);
	}
}

CStreamClientItem::CStreamClientItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) throw(ExMalformedPatch)
: IContentItem( root_item, parent, CItemType::E_STREAM_CLIENT )
{
	size_t lpos, rpos;
	lpos = 0;
	bool serialisationNeeded = true;

	try {
		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string typeStr = text.substr(lpos, rpos - lpos);
			if(typeStr.compare(CItemType::getString(m_item_type)) == 0 ) {
				// serialisation started with type string 'S' -> input string is suitable for m_text
				lpos = rpos + 1;
				m_text = text;
				serialisationNeeded = false;
			}
		}
		else {
			throw ExMalformedPatch("error parsing first field (expecting type string or 'service name'): terminating tab char is missing." ,-1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_service_name = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'service_name' field, terminating tab char is missing.", -1);
		}

		m_hash = hash(m_service_name);

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_host_name = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'host_name' field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_domain_name = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'domain_name' field, terminating tab char is missing.", -1);
		}

		string m_port_str;
		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_port_str = text.substr(lpos, rpos - lpos);
			m_port = CUtils::str2long(m_port_str);
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'port' field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_owner_session_name = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'OwnerSessionName' field, terminating tab char is missing.", -1);
		}

		string ena_str;
		rpos = text.find_first_of("\t\n", lpos);
		if(rpos != string::npos ) {
			ena_str = text.substr(lpos, rpos - lpos);
		}
		else {
			ena_str = text.substr(lpos, rpos - lpos);
		}

		if(ena_str.compare(DISABLED_STR) == 0) {
			m_enabled = false;
		}
		else {
			m_enabled = true;
		}

		m_root_item->setContentPtr(CItemType(CItemType::E_STREAM_CLIENT), this, m_hash );
		if(serialisationNeeded) {
			assembleText();
		}

		if(m_text.rfind('\n') == string::npos ) {
			m_text.append("\n");
		}

		if(m_parent) {
			m_parent->addChild(this, posInParent);
		}
		// m_hash is an index that does not change during live time of this object any more.
	}
	catch(std::invalid_argument& ex)
	{
		throw ExMalformedPatch(ex.what(), -1);
	}
}

CStreamClientItem::~CStreamClientItem() {
	m_root_item->delContentPtr(CItemType(CItemType::E_STREAM_CLIENT), m_hash );
}

//CStreamClientItem* CStreamClientItem::clone(const IContentItem& other, CRootItem* const root_item, CCategoryItem* const parent)
//{
//    assert(other.type() == CItemType::E_STREAM_CLIENT);
//    const CStreamClientItem* const otherPtr = reinterpret_cast<const CStreamClientItem* const>(&other);
//    CStreamClientItem* newItem = new CStreamClientItem(*otherPtr, root_item, parent);
//
//    newItem->m_domain_name = otherPtr->m_domain_name;
//    newItem->m_enabled = otherPtr->m_enabled;
//    newItem->m_hash = otherPtr->m_hash;
//    newItem->m_host_name = otherPtr->m_host_name;
//
//    return newItem;
//}


bool CStreamClientItem::operator==(const IContentItem& other) {
	if(other.type() != CItemType::E_STREAM_CLIENT ) {
		return false;
	}
	const CStreamClientItem * const rhs = static_cast<const CStreamClientItem*>(&other);

	if(m_service_name != rhs->m_service_name) {
		return false;
	}

	if(m_host_name != rhs->m_host_name) {
		return false;
	}

	if(m_domain_name != rhs->m_domain_name) {
		return false;
	}

	if(m_port != rhs->m_port) {
		return false;
	}

	if(m_enabled != rhs->m_enabled) {
		return false;
	}

	return true;
}

string CStreamClientItem::serialize(bool asDiff) {
	if(asDiff) {
		string diffline("+");
		diffline.append(m_text);
		return diffline;
	}
	else {
		return m_text;
	}
}

std::string CStreamClientItem::getDomainName() const {
	return m_domain_name;
}

void CStreamClientItem::setDomainName(std::string domainName) {
	m_domain_name = domainName;
	assembleText();
}

bool CStreamClientItem::isEnabled() const {
	return m_enabled;
}

void CStreamClientItem::setEnabled(bool enabled) {
	m_enabled = enabled;
	assembleText();
}

std::string CStreamClientItem::getHostName() const {
	return m_host_name;
}

void CStreamClientItem::setHostName(std::string hostName) {
	m_host_name = hostName;
	assembleText();
}

int32_t CStreamClientItem::getPort() const {
	return m_port;
}

void CStreamClientItem::setPort(int32_t port) {
	m_port = port;
	assembleText();
}

std::string CStreamClientItem::getOwnerSessionName() const {
	return m_owner_session_name;
}

void CStreamClientItem::setOwnerSessionName(std::string ownerSessionName) {
	m_owner_session_name = ownerSessionName;
	assembleText();
}

std::string CStreamClientItem::getServiceName() const {
	return m_service_name;
}

//void CStreamClientItem::setServiceName(std::string serviceName) {
//	m_service_name = serviceName;
//	assembleText();
//}

void CStreamClientItem::assembleText() {
	stringstream ss;

//	if( m_parent ) {
//		ss << m_parent->getPath();
//	}
	string ena = (m_enabled)?ENABLED_STR:DISABLED_STR;

	ss << CItemType::getString(m_item_type) << "\t" << m_service_name << "\t" << m_host_name << "\t" << m_domain_name << "\t" << m_port << "\t" << m_owner_session_name << "\t" << ena << endl;
	m_text = ss.str();
}
} // namespace muroa
