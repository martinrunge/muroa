/***************************************************************************
 *                                                                         *
 *   CItemBase.h                                                           *
 *                                                                         *
 *   This file is part of libmuroa++                                       *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>              *
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

#ifndef CITEMBASE_H_
#define CITEMBASE_H_

#include <string>
#include <vector>

#include "CItemType.h"

class IContentItem;
class CCategoryItem;
class CRootItem;

class CItemBase {
public:
	CItemBase(CRootItem *root_item, CCategoryItem*  parent, const CItemType::item_type_t type);
	virtual ~CItemBase();

protected:
	CItemBase(CRootItem *root_item, std::string text, const CItemType::item_type_t type);

public:
	inline std::string getName() const { return m_name; };

	inline CCategoryItem* getParent() const { return m_parent; };
	virtual void addChild(IContentItem* newChild, int pos = -1) = 0;
	virtual int numChildren() = 0;

	virtual CItemBase* childAt(unsigned row) = 0;
	virtual unsigned childPos(const CItemBase* const child) = 0;

	inline std::string getText() { return m_text; };
	virtual std::string serialize(bool asDiff = false) = 0;

	inline item_type_t type() { return m_item_type; };

protected:
	std::string m_name;

	CCategoryItem*  m_parent;

	std::string m_text;
	void replaceTabs(std::string& str);

	const CItemType::item_type_t m_item_type;

	CRootItem *m_root_item;

};

#endif /* CITEMBASE_H_ */
