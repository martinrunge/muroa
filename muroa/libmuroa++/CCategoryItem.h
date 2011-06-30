/***************************************************************************
 *
 *   CCategoryItem.h
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

#ifndef CCATEGORYITEM_H_
#define CCATEGORYITEM_H_

#include "CItemBase.h"

#include <string>
#include <sstream>

class CContentItem;


class CCategoryItem : public CItemBase {
public:
	CCategoryItem(CRootItem *root_item, std::string text = std::string(), CCategoryItem*  parent = 0);
	virtual ~CCategoryItem();

	inline std::string getPath() const { return m_path; };

	void addChild(IContentItem*  newContentItem, int pos = -1);

	IContentItem* getContentItem(unsigned pos);
	CCategoryItem* getCategoryItem(std::string name);
	CItemBase* childAt(unsigned row);
	unsigned childPos(const CItemBase* const child);

	int numChildren();
	int getNumContentItems();
	int getNumCategories();

	void delContentItem(int pos);
	void delCategory(CCategoryItem* categoryItem);

	std::string serialize(bool asDiff = false);
	std::string diff(const CCategoryItem* other);

	bool operator==(const CCategoryItem& other);
	inline bool operator!=(const CCategoryItem& other) {
		return !operator==(other);
	}

	static std::string getParentPath(std::string ownPath);
private:
	void addChild(CCategoryItem* newSubCategory);

	std::vector<CCategoryItem*>  m_sub_categories;
	std::vector<IContentItem*>   m_content_items;
	std::istringstream m_iss;

	int m_depth;
	std::string m_path;
};

struct CompareCategoriesByName {
public:
    bool operator () (const CCategoryItem* const lhs, const CCategoryItem* const rhs) { return lhs->getName() < rhs->getName(); }
    bool operator () (const CCategoryItem& lhs, const CCategoryItem& rhs) { return lhs.getName() < rhs.getName(); }
};


#endif /* CCATEGORYITEM_H_ */
