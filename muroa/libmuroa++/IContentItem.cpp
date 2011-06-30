/*
 * IContentItem.cpp
 *
 *  Created on: 29 Jun 2011
 *      Author: martin
 */

#include "IContentItem.h"

#include "CMediaItem.h"

IContentItem::IContentItem(CRootItem *root_item, CCategoryItem*  parent, const item_type_t type) :
   CItemBase(root_item, parent, type)
{

}

IContentItem::~IContentItem() {

}

IContentItem* IContentItem::itemFactory(const item_type_t type, CRootItem *root_item, CCategoryItem *parent, const unsigned posInParent) {
	switch(type) {
	case CItemType::E_ROOT:
		return 0;
		break;
	case CItemType::E_CAT:
		return 0;
		break;
	case CItemType::E_MEDIAITEM:
		return new CMediaItem(root_item, parent, posInParent);
		break;
	case CItemType::E_PLAYLISTITEM:
		break;
	case CItemType::E_NEXTLISTITEM:
		break;
	default:
		return 0;
		break;
	}
}

IContentItem* IContentItem::itemFactory(const item_type_t type, CRootItem *root_item, std::string text, CCategoryItem *parent, const unsigned posInParent) {
	switch(type) {
	case CItemType::E_ROOT:
		return 0;
		break;
	case CItemType::E_CAT:
		return 0;
		break;
	case CItemType::E_MEDIAITEM:
		return new CMediaItem(root_item, text, parent, posInParent);
		break;
	case CItemType::E_PLAYLISTITEM:
		break;
	case CItemType::E_NEXTLISTITEM:
		break;
	default:
		return 0;
		break;
	}
}


bool IContentItem::operator==(const IContentItem& other) {
	return true;
}

