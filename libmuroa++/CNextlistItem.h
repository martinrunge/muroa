/***************************************************************************
 *
 *   CNextlistItem.h
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

#ifndef CNEXTLISTITEM_H_
#define CNEXTLISTITEM_H_

#include "IContentItem.h"

class CPlaylistItem;

class CNextlistItem: public IContentItem {
public:
	CNextlistItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent = -1);
	CNextlistItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent = -1);
	virtual ~CNextlistItem();

	inline uint32_t getPlaylistItemHash() { return m_playlistitem_hash; };
	void setPlaylistItem(CPlaylistItem* plItem);
	void setPlaylistItemHash(uint32_t hash);

	bool operator==(const IContentItem& other);
	inline bool operator!=(const IContentItem& other){ return !operator==(other); };

	std::string serialize(bool asDiff = false);

private:
	uint32_t m_playlistitem_hash;
	static uint32_t m_next_free_id;

	void assembleText();

};

#endif /* CNEXTLISTITEM_H_ */
