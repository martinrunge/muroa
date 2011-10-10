/*
 * CRootItem.h
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#ifndef CROOTITEM_H_
#define CROOTITEM_H_

#include "MuroaExceptions.h"

#include "CItemType.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <stdexcept>

class CItemBase;
class CCategoryItem;
class IContentItem;


class CRootItem {
public:
	CRootItem();
	virtual ~CRootItem();

	CCategoryItem* addCategory(std::string name, CCategoryItem* parent = 0);

//	IContentItem* addContentItem(CCategoryItem* parent = 0, int posInParent = -1);
	IContentItem* addEmptyContentItem(CItemType type, CCategoryItem* parent, int posInParent = -1);
	IContentItem* addContentItem(std::string textWoPath, CCategoryItem* parent, int posInParent = -1);
	IContentItem* addContentItem(std::string text, int posInParent = -1);

	CCategoryItem* getBase() const { return m_base; };

	std::string serialize();
	void deserialize(std::string text);

	std::string diff(const CRootItem& other);
	void patch(std::string diff) throw(std::invalid_argument, MalformedPatchEx);

	bool operator==(const CRootItem& other);

	virtual bool beginInsertItems(const int pos, const int count, const CCategoryItem* parent);
	virtual bool endInsertItems(void);
	virtual bool beginRemoveItems(const int pos, const int count, const CCategoryItem* parent);
	virtual bool endRemoveItems(void);

	// implementation traverses object tree and compares names
	//CCategoryItem* getItemPtr(std::string path);

	CCategoryItem* getItemPtr(std::string path) {
    	std::map<std::string, CCategoryItem*>::iterator it;
    	it = m_category_map.find(path);
    	if(it == m_category_map.end()) {
    		return 0;
    	}
    	else {
    		return it->second;
    	}
   	}

	inline void setItemPtr(std::string path, CCategoryItem* itemPtr) {
		std::pair<std::map<std::string, CCategoryItem*>::iterator,bool> ret;
		ret = m_category_map.insert(std::pair<std::string,CCategoryItem*>(path, itemPtr));
		if (ret.second==false)
		{
		    std::cout << "element 'z' already existed";
		    std::cout << " with a value of " << ret.first->second << std::endl;
		}
	}

	inline void delItemPtr(std::string path) {
		m_category_map.erase(path);
	}

	IContentItem* getContentPtr(const CItemType& type, const uint32_t hash);
	void setContentPtr(const CItemType& type, IContentItem* ptr, const uint32_t hash);
	void delContentPtr(const CItemType& type, const uint32_t hash);
private:

	std::map<std::string, CCategoryItem*> m_category_map;

	CCategoryItem* m_base;

	std::vector< std::map< uint32_t, IContentItem* > > m_content_maps;

	std::string stripFirstSection(std::string& text);
	CItemType getItemType(std::string& text);
	CCategoryItem* mkPath(std::string path);
};

#endif /* CROOTITEM_H_ */
