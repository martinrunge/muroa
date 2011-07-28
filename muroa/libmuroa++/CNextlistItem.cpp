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
#include <sstream>
using namespace std;

uint32_t CNextlistItem::m_next_free_id = 0;

CNextlistItem::CNextlistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{
    m_hash = m_next_free_id++;
	m_root_item->setContentPtr(CItemType(CItemType::E_PLAYLISTITEM), this, m_hash );
}

CNextlistItem::CNextlistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{
	m_text = text;
	size_t lpos, rpos;
	lpos = 1;

	rpos = m_text.find('\t', lpos);
	string playlistitemHashStr = text.substr(lpos, rpos - lpos);
	m_playlistitem_hash = strtol(playlistitemHashStr.c_str(), NULL, 10);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	string hashStr = text.substr(lpos, rpos - lpos);
	m_hash = strtol(hashStr.c_str(), NULL, 10);
	lpos = rpos + 1;

	assembleText();

	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
	m_root_item->setContentPtr(CItemType(CItemType::E_NEXTLISTITEM), this, m_hash );
}

CNextlistItem::~CNextlistItem() {
}

void CNextlistItem::setPlaylistItem(CPlaylistItem* plItem) {
	m_playlistitem_hash = plItem->getHash();
	assembleText();
}

void CNextlistItem::setPlaylistItemHash(uint32_t hash) {
	m_playlistitem_hash = hash;
	assembleText();
}


bool CNextlistItem::operator==(const IContentItem& other) {
	if(other.type() != CItemType::E_NEXTLISTITEM ) {
		return false;
	}
	const CNextlistItem * const rhs = static_cast<const CNextlistItem*>(&other);

	if(m_playlistitem_hash != rhs->m_playlistitem_hash) {
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

	if( m_parent ) {
		ss << m_parent->getPath();
	}
	ss << "\tN\t" << m_playlistitem_hash << "\t" << m_hash;
	m_text = ss.str();
}

