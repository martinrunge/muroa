/*
 * CRootItem.h
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#ifndef CROOTITEM_H_
#define CROOTITEM_H_

#include "MuroaExceptions.h"

#include <string>
#include <map>
#include <iostream>

#include <stdexcept>

class CItemBase;
class CCategoryItem;
class CMediaItem;

class CRootItem {
public:
	CRootItem();
	virtual ~CRootItem();


	CCategoryItem* addCategory(std::string name, CCategoryItem* parent = 0);

	CMediaItem* addMediaItem(CCategoryItem* parent = 0, int posInParent = -1);
	CMediaItem* addMediaItem(std::string textWoPath, CCategoryItem* parent, int posInParent = -1);
	CMediaItem* addMediaItem(std::string text, int posInParent = -1);

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

	CCategoryItem* getItemPtr(std::string path);

	//	CCategoryItem* getItemPtr(std::string path) {
//    	std::map<std::string, CCategoryItem*>::iterator it;
//    	it = m_map.find(path);
//    	if(it == m_map.end()) {
//    		return 0;
//    	}
//    	else {
//    		return it->second;
//    	}
//   	}

//	inline void setItemPtr(std::string path, CCategoryItem* itemPtr) {
//		std::pair<std::map<std::string, CCategoryItem*>::iterator,bool> ret;
//		ret = m_map.insert(std::pair<std::string,CCategoryItem*>(path, itemPtr));
//		if (ret.second==false)
//		{
//		    std::cout << "element 'z' already existed";
//		    std::cout << " with a value of " << ret.first->second << std::endl;
//		}
//	}

private:
	std::map<std::string, CCategoryItem*> m_map;
	CCategoryItem* m_base;

	CCategoryItem* mkPath(std::string path);
	long str2long(std::string str) throw(std::invalid_argument);
};

#endif /* CROOTITEM_H_ */
