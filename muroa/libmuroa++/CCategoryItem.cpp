/***************************************************************************
 *
 *   CCategoryItem.cpp
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

#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CDiff.h"

#include <typeinfo>
#include <iostream>
#include <cassert>

using namespace std;

CCategoryItem::CCategoryItem(string text, CCategoryItem*  parent) : CItemBase(parent, E_CAT)
{
	replaceTabs(text);
	m_name = text;

	if(m_parent) {
		m_path = m_parent->getPath();
		m_path.append("/");
	}
	m_path.append(m_name);
	if(m_parent) {
		m_parent->addChild(this);
	}
}


CCategoryItem::~CCategoryItem() {
	std::vector<CCategoryItem*>::iterator cit;
	for(cit = m_sub_categories.begin(); cit != m_sub_categories.end(); cit++ ) {
		delete *cit;
	}

	std::vector<CMediaItem*>::iterator mit;
	for(mit = m_media_items.begin(); mit != m_media_items.end(); mit++ ) {
		delete *mit;
	}
}

void CCategoryItem::addChild(CCategoryItem* newSubCategory) {
	m_sub_categories.push_back(newSubCategory);
}

void CCategoryItem::addChild(CMediaItem*  newMediaItem, int pos) {
	if(pos == -1) {
		m_media_items.push_back(newMediaItem);
	}
	else {
		m_media_items.insert( m_media_items.begin() + pos, newMediaItem );
	}
}

CMediaItem* CCategoryItem::getMediaItem(int pos) {
	return m_media_items[pos];
}

void CCategoryItem::delMediaItem(int pos) {
	m_media_items.erase(m_media_items.begin() + pos);
}


void CCategoryItem::delCategory(CCategoryItem* categoryItem) {
	std::vector<CCategoryItem*>::iterator cit;
	for(cit = m_sub_categories.begin(); cit != m_sub_categories.end(); cit++ ) {
		if(categoryItem == *cit) {
			m_sub_categories.erase(cit);
			break;
		}
	}
}


string CCategoryItem::serialize(bool asDiff) {
	string result;
	// result.append(m_path);
	// result.append("\n");
	std::vector<CCategoryItem*>::iterator cit;
	for(cit = m_sub_categories.begin(); cit != m_sub_categories.end(); cit++ ) {
		result.append((*cit)->serialize(asDiff));
	}

	if(asDiff) {
		ostringstream oss;
		oss << "+++ " << getPath() << endl;
		oss << "@@ -0,0 +1," << m_media_items.size() << endl;
		result.append(oss.str());
	}
	std::vector<CMediaItem*>::iterator mit;
	for(mit = m_media_items.begin(); mit != m_media_items.end(); mit++ ) {
		result.append((*mit)->serialize(asDiff));
	}

	return result;
}

string CCategoryItem::diff(const CCategoryItem* other) {
	string diff;

	vector<CCategoryItem*>::const_iterator cit;
	vector<CCategoryItem*>::const_iterator other_cit = other->m_sub_categories.begin();

	string childpath;
	for(cit = m_sub_categories.begin(); cit != m_sub_categories.end(); cit++ ) {

		childpath = (*cit)->getPath();
		// cerr << "outer for: cit: " << childpath << " other cit: " << (*other_cit)->getPath() << endl;

		if( other_cit == other->m_sub_categories.end() || childpath.compare( (*other_cit)->getPath()) < 0 ) {
			// old is smaller than new or new has no sub_categories left-> this entry disappeared in new
			ostringstream oss;
			oss << "--- " << (*cit)->getPath() << endl;
			diff.append(oss.str());
			continue;
		}

		while( other_cit != other->m_sub_categories.end()) {
			string other_childpath = (*other_cit)->getPath();

			// cerr << "inner while: cit: " << childpath << " other cit: " << other_childpath << " ";

			if( childpath.compare( other_childpath ) < 0  ) {
				// cerr << "break " << endl;
				break;
			}
			if( childpath.compare( other_childpath ) > 0  ) {
				string result = (*other_cit)->serialize(true);
				diff.append(result);
			}
			else {
				// cerr << " doing diff: " << endl;
				// other_cit is either not present in cit or just differs
				string result = (*cit)->diff(*other_cit);
				diff.append(result);
			}
			other_cit++;
		}
	}

	while( other_cit != other->m_sub_categories.end()) {
		string other_childpath = (*other_cit)->getPath();

		// cerr << "later while: other cit: " << other_childpath << " ";
		// cerr << " doing diff anyway: " << endl;
		// other_cit is either not present in cit or just differs
		string result = (*other_cit)->serialize(true);
		diff.append(result);

		other_cit++;
	}


	string ltext, rtext;
	vector<CMediaItem*>::const_iterator mit;
	vector<CMediaItem*>::const_iterator other_mit;

	for(mit = m_media_items.begin(); mit != m_media_items.end(); mit++) {
		ltext.append((*mit)->getText());
	}

	for(other_mit = other->m_media_items.begin(); other_mit != other->m_media_items.end(); other_mit++) {
		rtext.append((*other_mit)->getText());
	}

	if(ltext.compare(rtext) != 0 ) {
		ostringstream oss;
		oss << "+++ " << getPath() << endl;
		diff.append(oss.str());
		CDiff differ;
		string result;
		result = differ.diff(ltext, rtext);
		diff.append(result);
	}
	return diff;
}


bool CCategoryItem::operator==(const CCategoryItem& other) {

	if(m_path != other.m_path) {
		return false;
	}

	if(m_name != other.m_name) {
		return false;
	}

	if( m_sub_categories.size() != other.m_sub_categories.size() ) {
		return false;
	}

	if( m_media_items.size() != other.m_media_items.size() ) {
		return false;
	}

	vector<CCategoryItem*>::const_iterator cit = m_sub_categories.begin();
	vector<CCategoryItem*>::const_iterator other_cit = other.m_sub_categories.begin();
	while( cit != m_sub_categories.end() && other_cit != other.m_sub_categories.end()) {
		if( *(*cit) != *(*other_cit) ) {
			return false;
		}
		cit++;
		other_cit++;

	}

	vector<CMediaItem*>::const_iterator mit = m_media_items.begin();
	vector<CMediaItem*>::const_iterator other_mit = other.m_media_items.begin();
	while( mit != m_media_items.end() && other_mit != other.m_media_items.end()) {
		if( *(*mit) != *(*other_mit) ) {
			return false;
		}
		mit++;
		other_mit++;

	}
	return true;
}

string CCategoryItem::getParentPath(string ownPath) {
	size_t rpos = ownPath.rfind('\t', 0);
	if(rpos == string::npos ) {
		return "";
	}
	else {
		return ownPath.substr(0, rpos);
	}
}

