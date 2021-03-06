/***************************************************************************
 *
 *   CNextlistItem.cpp
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

#include "CNextlistItem.h"
#include "CRootItem.h"
#include "CCategoryItem.h"
#include "CPlaylistItem.h"
#include "CUtils.h"
#include <sstream>
#include <cassert>

using namespace std;
namespace muroa {

uint32_t CNextlistItem::m_next_free_id = 0;

CNextlistItem::CNextlistItem(const CNextlistItem& other, CRootItem *root_item, CCategoryItem*  parent):
               IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{
    m_hash = other.m_hash;
    m_mediaitem_hash = other.m_mediaitem_hash;
    m_name = other.m_name;
    m_text = other.m_text;

    m_root_item->setContentPtr(CItemType(CItemType::E_NEXTLISTITEM), this, m_hash );
}

CNextlistItem::CNextlistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{
    m_hash = m_next_free_id++;
	m_root_item->setContentPtr(CItemType(CItemType::E_NEXTLISTITEM), this, m_hash );
}

CNextlistItem::CNextlistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) throw(ExMalformedPatch)
: IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{
	size_t lpos, rpos;
	lpos = 0;
    m_hash = m_next_free_id++;
    bool serialisationNeeded = true;

	try {
		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string typeStr = text.substr(lpos, rpos - lpos);
			if(typeStr.compare(CItemType::getString(m_item_type)) == 0 ) {
				lpos = rpos + 1;
				m_text = text;
				serialisationNeeded = false;
			}
		}
		else {
			throw ExMalformedPatch("error parsing first field (expecting type string or 'media item hash' field): terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string mediaitemHashStr = text.substr(lpos, rpos - lpos);
			m_mediaitem_hash = CUtils::str2uint32(mediaitemHashStr.c_str());
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'media item hash' field, terminating tab char is missing.", -1);
		}

		string playlistIdStr;
		rpos = text.find_first_of("\t\n", lpos);
		if( rpos != string::npos ) {
			playlistIdStr = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			playlistIdStr = text.substr(lpos);
		}
		m_playlistitem_hash = CUtils::str2uint32(playlistIdStr.c_str());

		if(serialisationNeeded) {
		   assembleText();
		   // from here on "m_text" is surely valid
		}
		m_root_item->setContentPtr(CItemType(CItemType::E_NEXTLISTITEM), this, m_hash );

		if(m_text.rfind('\n') == string::npos ) {
			m_text.append("\n");
		}

		if(m_parent) {
			m_parent->addChild(this, posInParent);
		}
		// m_hash is an index that does not change during livetime of this object any more.
	}
	catch(std::invalid_argument& ex)
	{
		throw ExMalformedPatch(ex.what(), -1);
	}
}

CNextlistItem::~CNextlistItem() {
	m_root_item->delContentPtr(CItemType(CItemType::E_NEXTLISTITEM), m_hash );
}


//CNextlistItem* CNextlistItem::clone(const IContentItem& other, CRootItem* root_item, CCategoryItem* parent)
//{
//    assert(other.type() == CItemType::E_NEXTLISTITEM);
//    const CNextlistItem* const otherPtr = reinterpret_cast<const CNextlistItem* const>(&other);
//    CNextlistItem* newItem = new CNextlistItem(*otherPtr, root_item, parent);
//
//    return newItem;
//}

void CNextlistItem::setPlaylistItem(CPlaylistItem* plItem) {
	m_playlistitem_hash = plItem->getHash();
	assembleText();
}

void CNextlistItem::setPlaylistItemHash(uint32_t hash) {
	m_playlistitem_hash = hash;
	assembleText();
}

void CNextlistItem::setMediaItemHash(uint32_t hash) {
	m_mediaitem_hash = hash;
	assembleText();
}



bool CNextlistItem::operator==(const IContentItem& other) {
	if(other.type() != CItemType::E_NEXTLISTITEM ) {
		return false;
	}
	const CNextlistItem * const rhs = static_cast<const CNextlistItem*>(&other);

	if(m_mediaitem_hash != rhs->m_mediaitem_hash) {
		return false;
	}

	return true;
}

string CNextlistItem::serialize(bool asDiff) {
	if(asDiff) {
		string diffline("+");
		diffline.append(m_text);
		return diffline;
	}
	else {
		return m_text;
	}
}

void CNextlistItem::assembleText() {
	stringstream ss;

//	if( m_parent ) {
//		ss << m_parent->getPath();
//	}
	ss << "N\t" << m_mediaitem_hash << "\t" << m_playlistitem_hash << endl;
	m_text = ss.str();
}
} // namespace muroa
