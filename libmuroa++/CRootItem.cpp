/*
 * CRootItem.cpp
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#include "CRootItem.h"
#include "IItemModel.h"

#include "CCategoryItem.h"
#include "IContentItem.h"
#include "CDiff.h"
#include "CUtils.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/regex.hpp>

#include <cassert>
#include <cstring>

using namespace std;
namespace muroa {

CRootItem::CRootItem(uint32_t rev) : m_base(0),
                                     m_revision(rev),
                                     m_referenced_mediaCol_rev(0),
                                     m_referenced_playlist_rev(0)
{
	reset();
}

CRootItem::CRootItem(const CRootItem& other) {

    for(int i=0; i < CItemType::numTypes(); i++) {
        m_content_maps.push_back( new map<uint32_t, IContentItem*>() );
    }

    m_base = new CCategoryItem(*(other.m_base), this, 0);

}


CRootItem::~CRootItem() {
	beginRemoveItems(0, m_base->getNumCategories() + m_base->getNumContentItems(), m_base);
	delete m_base;
	m_content_maps.clear();
	endRemoveItems();
}

void CRootItem::reset() {
	if(m_base != 0) {
		beginRemoveItems(0, m_base->getNumCategories() + m_base->getNumContentItems(), m_base);
		delete m_base;
		m_content_maps.clear();
		m_base = new CCategoryItem(this, "", 0);
		endRemoveItems();
	}
	else {
		m_base = new CCategoryItem(this, "", 0);
	}
	// setCategoryPtr("/", m_base);
	for(int i=0; i < CItemType::numTypes(); i++) {
		m_content_maps.push_back( new map<uint32_t, IContentItem*>() );
	}
}

//void CRootItem::clear() {
//	beginRemoveItems(0, m_base->getNumCategories() + m_base->getNumContentItems(), m_base);
//	delete m_base;
//	m_content_maps.clear();
//	endRemoveItems();
//}


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

//IContentItem* CRootItem::addContentItem(CCategoryItem* parent, int posInParent) {
//	if (parent == 0)  {
//		parent = m_base;
//	}
//	if(posInParent == -1) {
//		posInParent = parent->numChildren();
//	}
//	beginInsertItems(posInParent, 1, parent );
//	IContentItem* newItem = IContentItem::itemFactory( CItemType::E_MEDIAITEM, this, parent, posInParent);
//	endInsertItems();
//	return newItem;
//}

IContentItem* CRootItem::addEmptyContentItem(CItemType type, CCategoryItem* parent, int posInParent) {
	if (parent == 0)  {
		parent = m_base;
	}
	if(posInParent == -1) {
		posInParent = parent->numChildren();
	}
	beginInsertItems(posInParent, 1, parent );
	IContentItem* newItem = IContentItem::itemFactory( type, this, parent, posInParent);
	endInsertItems();
	return newItem;
}


IContentItem* CRootItem::addContentItem(string textWoPath, CCategoryItem* parent, int posInParent) throw (ExMalformedPatch)
{
	CItemType itemType = getItemType(textWoPath);
	if(posInParent == -1) {
		posInParent = parent->numChildren();
	}
	beginInsertItems(posInParent, 1, parent );
	IContentItem* newItem = IContentItem::itemFactory( itemType, this, textWoPath, parent, posInParent);
	endInsertItems();
	return newItem;
}

IContentItem* CRootItem::addContentItem(IContentItem* item, CCategoryItem* parent, int posInParent)
{
	if(posInParent == -1) {
		posInParent = parent->numChildren();
	}
	beginInsertItems(posInParent, 1, parent );
	parent->addChild(item, posInParent);

	setContentPtr(CItemType(item->type()), item, item->getHash());

	endInsertItems();

	return item;
}

IContentItem* CRootItem::addContentItem(string text, int posInParent) throw (ExMalformedPatch)
{
	string path = stripFirstSection(text);
	CItemType itemType = getItemType(text);
	if(itemType.getType() == CItemType::E_INVAL ) {
		throw ExMalformedPatch("invalid item type", -1);
	}

	CCategoryItem* parent = getCategoryPtr(path);
	if(parent == 0) {
		parent = mkPath(path);
	}

//	if(posInParent == -1) {
//		posInParent = parent->numChildren();
//	}
	beginInsertItems(posInParent, 1, parent );
	IContentItem* newItem = IContentItem::itemFactory( itemType, this, text, parent, posInParent);
	endInsertItems();
	return newItem;
}

IContentItem* CRootItem::getContentPtr(const CItemType& type, const uint32_t hash) {
	map<uint32_t, IContentItem*>::iterator it;
	map<uint32_t, IContentItem*>* content_map = m_content_maps[type.getType()];

	it = content_map->find( hash );
	if(it == content_map->end()) {
		return 0;
	}
	else {
		return it->second;
	}
}

void CRootItem::setContentPtr(const CItemType& type, IContentItem* ptr, const uint32_t hash) {
	pair<map<uint32_t, IContentItem*>::iterator, bool> ret;
	map<uint32_t, IContentItem*>* content_map = m_content_maps[type.getType()];

	ret = content_map->insert(pair<uint32_t, IContentItem*>(hash, ptr));
	if (ret.second==false)
	{
	    std::cerr << "setContentPtr: element '" << ret.first->first << "' already existed";
	    std::cerr << " with a value of " << ret.first->second << std::endl;
	}
}

void CRootItem::delContentPtr(const CItemType& type, const uint32_t hash) {
	map<uint32_t, IContentItem*>* content_map = m_content_maps[type.getType()];
	content_map->erase(hash);
}


void CRootItem::deserialize(std::string text) throw(ExMalformedPatch) {
	istringstream iss(text);
	string line;
	line.reserve(4096);

	reset();

	CItemBase* currentCategory = 0;

	while(!iss.eof() && !iss.fail()) {
		getline(iss, line);
		if(iss.fail()) {
			cerr << "CRootItem::deserialize: Error reading lines." << endl;
			return;
		}
		if( line.size() < 3)  continue;
		line.append("\n");
		IContentItem* contItem = addContentItem(line);
	}
}

void CRootItem::fromFile(std::string filename) throw(ExMalformedPatch) {
	ifstream ifs(filename);
	int lineNr = 0;

	reset();

	CItemBase* currentCategory = 0;

	while(!ifs.eof() && !ifs.fail()) {
		string line;
		line.reserve(4096);  // should be enough
		getline(ifs, line);
		lineNr++;
		if(lineNr <= 3) {
			int pos = line.find("=");
			line = line.substr(pos+1, line.size() - pos - 1);
			try {
				uint32_t value = CUtils::str2uint32(line);

				switch(lineNr) {
				case 1:
					m_revision= value;
					break;
				case 2:
					m_referenced_mediaCol_rev= value;
					break;
				case 3:
					m_referenced_playlist_rev = value;
					break;
				default:
					break;
				}
			}
			catch(std::invalid_argument& ex) {
				throw ExMalformedPatch(ex.what(), lineNr);
			}

		}
		else {
			if( line.size() < 3)  continue;
			line.append("\n");
			IContentItem* contItem = addContentItem(line);
		}
	}
}

std::string CRootItem::serialize(string filename ) {
	string serialisation = m_base->serialize();
	if(! filename.empty()) {
		ofstream ofs(filename);
		ofs << "revision=" << m_revision << endl;
		ofs << "referencedMediaColRev=" << m_referenced_mediaCol_rev << endl;
		ofs << "referencedPlaylistRev=" << m_referenced_playlist_rev << endl;
		ofs << serialisation;
		ofs.close();
	}
	return serialisation;
}

string CRootItem::diff(const CRootItem& other) {
//	CDiff differ;
//	string diff = differ.diff( serialize(), other.serialize() );

	return m_base->diff(other.m_base);
}

void CRootItem::patch(std::string diff) throw(ExMalformedPatch) {
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
			parent = getCategoryPtr(path);
			if(parent == 0) {
				parent = mkPath(path);
			}
			new_category = true;
			chunkSizeSum = 0;
		}
		else if(line.find("--- ") == 0) {
			// remove a complete category
			string path = line.substr(4, line.size() - 4);
			CCategoryItem *categoryToRemove = getCategoryPtr(path);
			if(categoryToRemove == 0) {
				ostringstream oss;
				oss << "CRootItem::patch (__FILE__:__LINE__): the category item to be removed was not be found in current collection(" << path << ").";
				throw ExMalformedPatch(oss.str(), patchLineNr);
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

			try
			{
				oldStart = CUtils::str2long( oldStartStr );
				oldLen = CUtils::str2long( oldLenStr );
				newStart = CUtils::str2long( newStartStr );
				newLen = CUtils::str2long( newLenStr );
			}
			catch(std::invalid_argument& ex)
			{
				throw ExMalformedPatch(ex.what(), lineNr);
			}

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
					IContentItem* newItem = addContentItem(content, lineNr - 1);
					break;
				}
				case '-': //remove
				{
					string path = stripFirstSection(content);
					parent = getCategoryPtr(path);
					if(parent == 0) {
						ostringstream oss;
						oss << "error removing item: unknown parent category '" << path << "'";
						throw ExMalformedPatch(oss.str(), patchLineNr);
					}

					IContentItem *contItem = parent->getContentItem(lineNr - 1);
					string itemText = contItem->getText();
					if(itemText.compare(0, itemText.size() - 1, content) != 0 )	// possible error:
					{
						cerr << "Error when removing line " << lineNr << " :" << endl;
						cerr << "line expected from diff : "<< content << endl;
						cerr << "differs form collection : " << contItem->getText();
					}
					parent->delContentItem(lineNr - 1);
					lineNr--;
					break;
				}
				case ' ': //check
				{
					IContentItem *contItem = parent->getContentItem(lineNr - 1);
					string itemText = contItem->getText();
					if(itemText.compare(0, itemText.size() - 1, content) != 0 )	// possible error:
					{
						cerr << "Error when keeping line " << lineNr << " :" << endl;
						cerr << "line expected from diff : "<< content << endl;
						cerr << "differs form collection : " << contItem->getText();
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
	set<IItemModel*>::iterator it;
	for(it = m_connected_ItemModels.begin(); it != m_connected_ItemModels.end(); it++ ) {
		(*it)->beginInsertItems(pos, count, parent);
	}
	return true;
}

bool CRootItem::endInsertItems(void) {
	set<IItemModel*>::iterator it;
	for(it = m_connected_ItemModels.begin(); it != m_connected_ItemModels.end(); it++ ) {
		(*it)->endInsertItems();
	}
	return true;
}

bool CRootItem::beginRemoveItems(const int pos, const int count, const CCategoryItem* parent) {
	set<IItemModel*>::iterator it;
	for(it = m_connected_ItemModels.begin(); it != m_connected_ItemModels.end(); it++ ) {
		(*it)->beginRemoveItems(pos, count, parent);
	}
	return true;
}

bool CRootItem::endRemoveItems(void) {
	set<IItemModel*>::iterator it;
	for(it = m_connected_ItemModels.begin(); it != m_connected_ItemModels.end(); it++ ) {
		(*it)->endRemoveItems();
	}
	return true;
}

CCategoryItem* CRootItem::getCategoryPtr(std::string path) {
	std::map<std::string, CCategoryItem*>::iterator it;
	it = m_category_map.find(path);
	if(it == m_category_map.end()) {
		return 0;
	}
	else {
		return it->second;
	}
	}

void CRootItem::setCategoryPtr(std::string path, CCategoryItem* itemPtr) {
	std::pair<std::map<std::string, CCategoryItem*>::iterator,bool> ret;
	ret = m_category_map.insert(std::pair<std::string,CCategoryItem*>(path, itemPtr));
	if (ret.second==false)
	{
	    std::cerr << "setCategoryPtr: element '" << ret.first->first << "' already existed";
	    std::cerr << " with a value of " << ret.first->second << std::endl;
	}
}

void CRootItem::delCategoryPtr(std::string path) {
	m_category_map.erase(path);
}

CRootItem::iterator CRootItem::begin() {


	return iterator(m_base);
}

CRootItem::iterator CRootItem::end() {
	return iterator(m_base, m_base->numChildren());
}


void CRootItem::connectItemModel(muroa::IItemModel* model) {
	m_connected_ItemModels.insert(model);
}

muroa::IItemModel* CRootItem::disconnectItemModel(muroa::IItemModel* model) {
	m_connected_ItemModels.erase(model);
}


//
//   alternative implementation without a cache
//CCategoryItem* CRootItem::getItemPtr(std::string path) {
//	size_t lpos = 1;
//	CCategoryItem* parent = m_base;
//
//	while( lpos < path.size() ) {
//		size_t rpos = path.find('/', lpos);
//		if(rpos == string::npos) {
//			if( lpos != path.size() ) {
//				rpos = path.size();
//			}
//			else {
//				break;
//			}
//		}
//
//		string catName = path.substr(lpos , rpos - lpos);
//
//		CCategoryItem* cItem = parent->getCategoryItem(catName);
//		if(cItem == 0) {
//			return 0;
//		}
//		parent = cItem;
//		lpos = rpos + 1;
//	}
//	return parent;
//}

std::string CRootItem::stripFirstSection(std::string& text) {
	string retval;
	size_t sec_off;
	size_t firstTabPos = text.find('\t');
	if(firstTabPos == string::npos) {
		// no tab at all -> return whole string and set text empty
		retval = text;
		text = "";
		return retval;
	}
	if(firstTabPos > 0) {
		// does not start with a tab -> return substring before tab and set text to rest
		retval = text.substr(0, firstTabPos);
		text = text.substr(firstTabPos + 1, text.size() - firstTabPos - 1);
	}
	else {
		// starts with a tab -> search second tab
		size_t secondTabPos = text.find('\t', firstTabPos + 1);
		if(secondTabPos == string::npos) {
			// no second tab -> return text without leading tab and set text empty
			retval = text.substr(1, text.size() - 1);
			text = "";
		}
		else {
			retval = text.substr(firstTabPos + 1, secondTabPos - firstTabPos - 1);
			text = text.substr(secondTabPos + 1, text.size() - secondTabPos - 1);
		}

	}
	return retval;
}

/***
 * get the first section of the text line (from beginning to first tab) and create a corresponding CItemType object.
 * The text is not modified.
 */
CItemType CRootItem::getItemType(const std::string& text) {
	int tabpos = text.find('\t');
	if( tabpos == string::npos ) {
		return CItemType(CItemType::E_INVAL);
	}
	string itemTypeStr = text.substr(0, tabpos);
	return CItemType(itemTypeStr);
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

		CCategoryItem* cItem = getCategoryPtr(catPath);
		if(cItem == 0) {
			cItem = new CCategoryItem( this, catName, parent);
			// setItemPtr(catPath, cItem);
		}
		parent = cItem;
		lpos = rpos + 1;
	}

	return static_cast<CCategoryItem*>(parent);
}
} // namespace muroa

