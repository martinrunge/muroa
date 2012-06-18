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
using namespace std;

uint32_t CPlaylistItem::m_next_free_id = 1;   // 0 is reserved

CPlaylistItem::CPlaylistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_PLAYLISTITEM )
{
	m_hash = m_next_free_id++;
	m_root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash );
	assembleText();
}

CPlaylistItem::CPlaylistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) throw(ExMalformedPatch)
: IContentItem( root_item, parent, CItemType::E_PLAYLISTITEM )
{

	m_text = text;
	// first section is handled by CItemBase
	size_t lpos, rpos;
	// lpos = m_text.find('\t', 1) + 1;
	lpos = 0;

	try {
		rpos = m_text.find('\t', lpos);
		if( rpos != string::npos ) {
			string mediaitemHashStr = text.substr(lpos, rpos - lpos);
			m_mediaitem_hash = CUtils::str2uint32(mediaitemHashStr.c_str());
			lpos = rpos + 1;
		}
		else {
			throw ExMalformedPatch("error parsing 'media item hash' field, terminating tab char is missing." ,-1);
		}

		string hashStr;
		rpos = m_text.find('\t', lpos);
		if( rpos != string::npos ) {
			hashStr = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
		}
		else {
			hashStr = text.substr(lpos);
		}
		m_hash = CUtils::str2uint32(hashStr.c_str());
		if(m_hash == 0){
			m_hash = m_next_free_id++;
		}

		assembleText();
		if(m_parent) {
			m_parent->addChild(this, posInParent);
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
		m_hash = m_next_free_id++;
	}
	else {
		m_hash = plID;
		if( m_next_free_id <= m_hash) {
			m_next_free_id = m_hash + 1;
		}
	}
	assembleText();
}

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
