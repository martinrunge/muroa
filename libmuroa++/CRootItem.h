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

#include "CRootItemIterator.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <stdexcept>

#include "CCategoryItem.h"

class CItemBase;
class IContentItem;

class CMediaItem;

#include <iterator>
using namespace std;


class CRootItem {
public:
	CRootItem(uint32_t rev = 0);
	virtual ~CRootItem();

	typedef muroa::CRootItemIterator iterator;
	iterator begin();
	iterator end();

	CCategoryItem* addCategory(std::string name, CCategoryItem* parent = 0);

//	IContentItem* addContentItem(CCategoryItem* parent = 0, int posInParent = -1);
	IContentItem* addEmptyContentItem(CItemType type, CCategoryItem* parent, int posInParent = -1);
	IContentItem* addContentItem(std::string textWoPath, CCategoryItem* parent, int posInParent = -1) throw (MalformedPatchEx);
	IContentItem* addContentItem(std::string text, int posInParent = -1) throw (MalformedPatchEx);

	IContentItem* addContentItem(IContentItem* item, CCategoryItem* parent, int posInParent = -1);

	CCategoryItem* getBase() const { return m_base; };

	void fromFile(std::string filename) throw(MalformedPatchEx);
	std::string serialize(std::string filename = "");
	void deserialize(std::string text) throw(MalformedPatchEx);

	std::string diff(const CRootItem& other);
	void patch(std::string diff) throw(MalformedPatchEx);

	bool operator==(const CRootItem& other);
	inline bool operator!=(const CRootItem& other) { return !operator==(other); };

	virtual bool beginInsertItems(const int pos, const int count, const CCategoryItem* parent);
	virtual bool endInsertItems(void);
	virtual bool beginRemoveItems(const int pos, const int count, const CCategoryItem* parent);
	virtual bool endRemoveItems(void);

	// implementation traverses object tree and compares names
	//CCategoryItem* getItemPtr(std::string path);

	CCategoryItem* getCategoryPtr(std::string path = "/");
    void setCategoryPtr(std::string path, CCategoryItem* itemPtr);
	void delCategoryPtr(std::string path);
	CCategoryItem* mkPath(std::string path);


	IContentItem* getContentPtr(const CItemType& type, const uint32_t hash);
	void setContentPtr(const CItemType& type, IContentItem* ptr, const uint32_t hash);
	void delContentPtr(const CItemType& type, const uint32_t hash);

	inline uint32_t getRevision() const { return m_revision; };
	inline void setRevision(const uint32_t  rev) { m_revision = rev; };

    uint32_t getReferencedMediaColRev() { return m_referenced_mediaCol_rev; };
    void setReferencedMediaColRev(uint32_t rev) { m_referenced_mediaCol_rev = rev; };

    uint32_t getReferencedPlaylistRev() { return m_referenced_playlist_rev; };
    void setReferencedPlaylistRev(uint32_t rev) { m_referenced_playlist_rev = rev; };

private:

	std::map<std::string, CCategoryItem*> m_category_map;

	CCategoryItem* m_base;

	std::vector< std::map< uint32_t, IContentItem* >* > m_content_maps;

	std::string stripFirstSection(std::string& text);
	CItemType getItemType(std::string& text);

	uint32_t m_revision;
    uint32_t m_referenced_mediaCol_rev;
    uint32_t m_referenced_playlist_rev;
};

#endif /* CROOTITEM_H_ */
