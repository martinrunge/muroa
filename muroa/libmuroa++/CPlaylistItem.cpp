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

#include <sstream>
using namespace std;

uint32_t CPlaylistItem::m_next_free_id = 0;

CPlaylistItem::CPlaylistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_PLAYLISTITEM )
{
	m_hash = m_next_free_id++;
	m_root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash );
	assembleText();
}

CPlaylistItem::CPlaylistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) :
		       IContentItem( root_item, parent, CItemType::E_PLAYLISTITEM )
{

	m_text = text;
	// first section is handled by CItemBase
	size_t lpos, rpos;
	// lpos = m_text.find('\t', 1) + 1;
	lpos = 1;

	rpos = m_text.find('\t', lpos);
	string mediaitemHashStr = text.substr(lpos, rpos - lpos);
	m_mediaitem_hash = strtol(mediaitemHashStr.c_str(), NULL, 10);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	string hashStr = text.substr(lpos, rpos - lpos);
	m_hash = strtol(hashStr.c_str(), NULL, 10);
	lpos = rpos + 1;

	assembleText();

	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
	m_root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash );
}

CPlaylistItem::~CPlaylistItem() {
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

	if( m_parent ) {
		ss << m_parent->getPath();
	}
	ss << "\tP\t" << m_mediaitem_hash << "\t" << m_hash;

	m_text = ss.str();
}

