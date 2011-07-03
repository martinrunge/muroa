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

using namespace std;

CNextlistItem::CNextlistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{

}

CNextlistItem::CNextlistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) :
               IContentItem( root_item, parent, CItemType::E_NEXTLISTITEM )
{

}

CNextlistItem::~CNextlistItem() {
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

	return "";
}
