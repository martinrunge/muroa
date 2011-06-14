/***************************************************************************
 *
 *   CItemBase.cpp
 *
 *   This file is part of libmuroa++                                  *
 *   Copyright (C) 2011 by martin                                      *
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

#include "CItemBase.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CItemBase::CItemBase(CCategoryItem*  parent, const item_type_t type) :
           m_parent(parent), m_item_type(type)
{
}

CItemBase::CItemBase(std::string text, const item_type_t type ): m_item_type(type) {
	int tabpos = text.find('\t', 0);
	int namepos = text.rfind('/', tabpos);

	m_name = text.substr(namepos, tabpos);
}


CItemBase::~CItemBase() {
	// traversing the tree for cleanup is done in CCategoryItem as CMediaItem may not have any children.
}


void CItemBase::replaceTabs(string& str) {
	std::replace( str.begin(), str.end(), '\t', ' ' );
	trim(str);
}

