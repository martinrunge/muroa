/*
 * CRootItem.cpp
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#include "CRootItem.h"

#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CDiff.h"

#include <iostream>
#include <sstream>

#include <boost/regex.hpp>

#include <cassert>
#include <cstring>

using namespace std;

CRootItem::CRootItem() {
	m_base = new CCategoryItem(this, "", 0);
	// setItemPtr(m_base->getName(), m_base);
}

CRootItem::~CRootItem() {
	delete m_base;
}

CCategoryItem* CRootItem::addCategory(string name, CCategoryItem* parent) {
	CCategoryItem* newItem;
	if(parent == 0) {
		newItem = new CCategoryItem(this, name, m_base);
	}
	else {
		newItem = new CCategoryItem(this, name, parent);
	}
	return newItem;
}

CMediaItem* CRootItem::addMediaItem(CCategoryItem* parent, int posInParent) {
	if (parent == 0)  {
		parent = m_base;
	}
	if(posInParent == -1) {
		posInParent = parent->numChildren();
	}
	beginInsertItems(posInParent, 1, parent );
	CMediaItem* newItem = new CMediaItem(this, parent, posInParent);
	endInsertItems();
	return newItem;
}

CMediaItem* CRootItem::addMediaItem(string textWoPath, CCategoryItem* parent, int posInParent) {
	if(posInParent == -1) {
		posInParent = parent->numChildren();
	}
	beginInsertItems(posInParent, 1, parent );
	CMediaItem* newItem = new CMediaItem(this, textWoPath, parent, posInParent);
	endInsertItems();
	return newItem;
}


CMediaItem* CRootItem::addMediaItem(string text, int posInParent) {

	size_t pathPos = text.find('\t');
	if(pathPos == string::npos) {
		return 0;
	}
	string path = text.substr(0, pathPos);

	CCategoryItem* parent = getItemPtr(path);
	if(parent == 0) {
		parent = mkPath(path);
	}

	string mItemText = text.substr(pathPos, text.size() - pathPos);

	if(posInParent == -1) {
		posInParent = parent->numChildren();
	}
	beginInsertItems(posInParent, 1, parent );
	CMediaItem* newItem = new CMediaItem(this, mItemText, parent, posInParent);
	endInsertItems();
	return newItem;
}


void CRootItem::deserialize(std::string text) {
	istringstream iss(text);
	char cline[4096];

	CItemBase* currentCategory = 0;

	while(!iss.eof()) {
		iss.getline(cline, 4096);
		if(iss.bad()) {
			cerr << "CRootItem::deserialize: Error reading lines." << endl;
		}

		CMediaItem* mItem = addMediaItem(cline);

		continue;

		if( strlen(cline) < 3)  continue;
		// assert( cline[0] != 0 && cline[1] != 0 && cline[2] != 0 );

	}
}

std::string CRootItem::serialize() {
	return m_base->serialize();
}

string CRootItem::diff(const CRootItem& other) {
//	CDiff differ;
//	string diff = differ.diff( serialize(), other.serialize() );

	return m_base->diff(other.m_base);
}

void CRootItem::patch(std::string diff) throw(std::invalid_argument, MalformedPatchEx) {
	istringstream iss(diff);

	CCategoryItem* parent = 0;

	bool new_category = true;
    boost::regex rx("^@@ -(\\d+),(\\d+)\\s+\\+(\\d+),(\\d+)\\s*@@$");
    int oldStart(0);
	int oldLen(0);
	int newStart(0);
	int newLen(0);

	int lineNr = 0;
	int patchLineNr = 0;
	int chunkSizeSum = 0;

	string line;
	while(!iss.eof()) {
		getline(iss, line);
		if(iss.bad()) {
			cerr << "CRootItem::patch: Error reading lines." << endl;
		}
		patchLineNr++;

		if( line.find("+++ ") == 0 ) {
			// new or modified category
			string path = line.substr(4, line.size() - 4);
			parent = getItemPtr(path);
			if(parent == 0) {
				parent = mkPath(path);
			}
			new_category = true;
			chunkSizeSum = 0;
		}
		else if(line.find("--- ") == 0) {
			// remove a complete category
			string path = line.substr(4, line.size() - 4);
			CCategoryItem *categoryToRemove = getItemPtr(path);
			if(categoryToRemove == 0) {
				ostringstream oss;
				oss << "CRootItem::patch (__FILE__:__LINE__): the category item to be removed was not be found in current collection(" << path << ").";
				throw MalformedPatchEx(oss.str(), patchLineNr);
			}
			CCategoryItem* parent = categoryToRemove->getParent();
			parent->delCategory(categoryToRemove);
			/// TODO really remove category here
			chunkSizeSum = 0;
			new_category = true;
		} else if( line.find("@@") == 0 ) {
			// diff chunk header
			boost::cmatch res;
		    boost::regex_search(line.c_str(), res, rx);

		    string oldStartStr = res[1];
			string oldLenStr = res[2];
			string newStartStr = res[3];
			string newLenStr = res[4];

			oldStart = str2long( oldStartStr );
			oldLen = str2long( oldLenStr );
			newStart = str2long( newStartStr );
			newLen = str2long( newLenStr );

			if(oldLen == 0) oldStart++;
			lineNr = oldStart + chunkSizeSum;

			chunkSizeSum += newLen - oldLen;

			new_category = false;
			// cerr << "- << oldStart << "," << oldLen << " + " << newStart << "," << newLen << endl;

		}
		else if( line.size() > 0 ) {
			char sign = line[0];
			string content = line.substr(1, line.size() - 1);
			assert(new_category == false);

			switch(sign){
				case '+': //insert
				{
					//cerr << "adding line : << lineNr << endl;;
					//cerr << "from diff : "<< content << endl;
					CMediaItem* newItem = addMediaItem(content, lineNr - 1);
					break;
				}
				case '-': //remove
				{
					CMediaItem *mItem = parent->getMediaItem(lineNr - 1);
					string itemText = mItem->getText();
					if(itemText.compare(0, itemText.size() - 1, content) != 0 )	// possible error:
					{
						cerr << "Error when removing line " << lineNr << " :" << endl;
						cerr << "line expected from diff : "<< content << endl;
						cerr << "differs form collection : " << mItem->getText();
					}
					parent->delMediaItem(lineNr - 1);
					lineNr--;
					break;
				}
				case ' ': //check
				{
					CMediaItem *mItem = parent->getMediaItem(lineNr - 1);
					string itemText = mItem->getText();
					if(itemText.compare(0, itemText.size() - 1, content) != 0 )	// possible error:
					{
						cerr << "Error when keeping line " << lineNr << " :" << endl;
						cerr << "line expected from diff : "<< content << endl;
						cerr << "differs form collection : " << mItem->getText();
					}
					break;
				}
				default:
					break;
			}
			lineNr++;
		}
	}
}




bool CRootItem::operator==(const CRootItem& other) {
	return (*m_base == *(other.m_base));
}


bool CRootItem::beginInsertItems(const int pos, const int count, const CCategoryItem* parent) {
	return true;
}

bool CRootItem::endInsertItems(void) {
	return true;
}

bool CRootItem::beginRemoveItems(const int pos, const int count, const CCategoryItem* parent) {
	return true;
}

bool CRootItem::endRemoveItems(void) {
	return true;
}


CCategoryItem* CRootItem::getItemPtr(std::string path) {
	size_t lpos = 1;
	CCategoryItem* parent = m_base;

	while( lpos < path.size() ) {
		size_t rpos = path.find('/', lpos);
		if(rpos == string::npos) {
			if( lpos != path.size() ) {
				rpos = path.size();
			}
			else {
				break;
			}
		}

		string catName = path.substr(lpos , rpos - lpos);

		CCategoryItem* cItem = parent->getCategoryItem(catName);
		if(cItem == 0) {
			return 0;
		}
		parent = cItem;
		lpos = rpos + 1;
	}
	return parent;
}


CCategoryItem* CRootItem::mkPath(string path) {
	size_t lpos = 1;
	CCategoryItem* parent = m_base;

	while( lpos < path.size() ) {
		size_t rpos = path.find('/', lpos);
		if(rpos == string::npos) {
			if( lpos != path.size() ) {
				rpos = path.size();
			}
			else {
				break;
			}
		}

		string catPath = path.substr(0, rpos);
		string catName = path.substr(lpos , rpos - lpos);

		CCategoryItem* cItem = getItemPtr(catPath);
		if(cItem == 0) {
			cItem = new CCategoryItem( this, catName, parent);
			// setItemPtr(catPath, cItem);
		}
		parent = cItem;
		lpos = rpos + 1;
	}

	return static_cast<CCategoryItem*>(parent);
}


long CRootItem::str2long(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	long iVal = strtol( str.c_str(), &endptr, 10);
	if (errno != 0 || *endptr != '\0' ) {
		throw invalid_argument("convert string to int");
	}
	return iVal;
}
