/***************************************************************************
 *
 *   CPlaylistItem.cpp
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

#include "CPlaylistItem.h"
#include "CRootItem.h"
#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CUtils.h"
#include <sstream>
#include <cassert>

using namespace std;

namespace muroa {

uint32_t CPlaylistItem::m_next_free_id = 1;   // 0 is reserved
IIdProvider* m_id_provider = 0;

uint32_t CPlaylistItem::getNextFreeID() {
	return m_next_free_id++;
}

void CPlaylistItem::setNextFreeID(uint32_t next_free_id) {
	m_next_free_id = next_free_id;
}

CPlaylistItem::CPlaylistItem(const CPlaylistItem& other, CRootItem* root_item, CCategoryItem* parent) :
               IContentItem(root_item, parent , CItemType::E_PLAYLISTITEM)
{
    m_hash = other.m_hash;
    m_mediaitem_hash = other.m_mediaitem_hash;
    root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash);
    m_text = other.m_text;
}


CPlaylistItem::CPlaylistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_PLAYLISTITEM )
{
	m_hash = getNextFreeID();
	m_root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash );
	assembleText();
}

CPlaylistItem::CPlaylistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) throw(ExMalformedPatch)
: IContentItem( root_item, parent, CItemType::E_PLAYLISTITEM )
{

	bool serialisationNeeded = true;

	// first section is handled by CItemBase
	size_t lpos, rpos;
	// lpos = m_text.find('\t', 1) + 1;
	lpos = 0;

	try {
		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string typeStr = text.substr(lpos, rpos - lpos);
			if(typeStr.compare(CItemType::getString(m_item_type)) == 0 ) {
				// serialisation started with type string 'P' -> input string is suitable for m_text
				lpos = rpos + 1;
				m_text = text;
				serialisationNeeded = false;
			}
		}
		else {
			throw ExMalformedPatch("error parsing first field (expecting type string or 'media item hash'): terminating tab char is missing." ,-1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string mediaitemHashStr = text.substr(lpos, rpos - lpos);
			m_mediaitem_hash = CUtils::str2uint32(mediaitemHashStr.c_str());
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'media item hash' field, terminating tab char is missing." ,-1);
		}

		string hashStr;
		rpos = text.find_first_of("\t\n", lpos);
		if( rpos != string::npos ) {
			hashStr = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			hashStr = text.substr(lpos);
		}
		m_hash = CUtils::str2uint32(hashStr.c_str());
		if(m_hash == 0){
			m_hash = getNextFreeID();
		}

		if(serialisationNeeded) {
			assembleText();
			// from here on "m_text" ist surely valid
		}

		if(m_parent) {
			m_parent->addChild(this, posInParent);
		}

		if(m_text.rfind('\n') == string::npos ) {
			m_text.append("\n");
		}

		// for CPlaylistItem, m_hash is an index number that never changes during
		// livetime of an object.
		m_root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash );
	}
	catch(std::invalid_argument& ex) {
		throw ExMalformedPatch(ex.what(), -1);
	}
}

CPlaylistItem::CPlaylistItem(uint32_t mediaItemHash, uint32_t plID )
              :IContentItem(0, 0, CItemType::E_PLAYLISTITEM),  m_mediaitem_hash(mediaItemHash)
{
	if(plID == 0) {
		m_hash = getNextFreeID();
	}
	else {
		m_hash = plID;
		if( getNextFreeID() <= m_hash) {
			setNextFreeID( m_hash + 1 );
		}
	}
	assembleText();
}

//CPlaylistItem* CPlaylistItem::clone(const IContentItem& other, CRootItem *root_item, CCategoryItem*  parent) {
//    assert(other.type() == CItemType::E_PLAYLISTITEM);
//    CPlaylistItem* newItem = new CPlaylistItem(root_item, parent);
//
//    newItem->m_mediaitem_hash = m_mediaitem_hash;
//
//    return newItem;
//}


void CPlaylistItem::setParent(CRootItem *root_item, CCategoryItem*  parent, int posInParent) {
	m_root_item = root_item;
	m_parent = parent;

	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
}


CPlaylistItem::~CPlaylistItem() {
		m_root_item->delContentPtr(CItemType(CItemType::E_PLAYLISTITEM), m_hash );  // would delete this
}

void CPlaylistItem::setMediaItemHash(uint32_t hash) {
	m_mediaitem_hash = hash;
	assembleText();
}

void CPlaylistItem::setMediaItem(CMediaItem* mediaitem) {
	m_mediaitem_hash = mediaitem->getHash();
	assembleText();
}


bool CPlaylistItem::operator==(const IContentItem& other) {
	if(other.type() != CItemType::E_PLAYLISTITEM ) {
		return false;
	}
	const CPlaylistItem * const rhs = reinterpret_cast<const CPlaylistItem*>(&other);

	if(m_mediaitem_hash != rhs->m_mediaitem_hash) {
		return false;
	}

	return true;
}


string CPlaylistItem::serialize(bool asDiff) {
	if(asDiff) {
		string diffline("+");
		diffline.append(m_text);
		return diffline;
	}
	else {
		return m_text;
	}
}

void CPlaylistItem::assembleText() {
	stringstream ss;

//	if( m_parent ) {
//		ss << m_parent->getPath();
//	}
	ss << "P\t" << m_mediaitem_hash << "\t" << m_hash << endl;

	m_text = ss.str();

}

} // namespace muroa

