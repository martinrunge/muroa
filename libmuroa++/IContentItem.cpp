/*
 * IContentItem.cpp
 *
 *  Created on: 29 Jun 2011
 *      Author: martin
 */

#include "IContentItem.h"

#include "CMediaItem.h"
#include "CPlaylistItem.h"
#include "CNextlistItem.h"
#include "CStreamClientItem.h"

namespace muroa {

IContentItem::IContentItem(CRootItem* const root_item, CCategoryItem*  const parent, const item_type_t type) :
   CItemBase(root_item, parent, type)
{

}

IContentItem::~IContentItem() {

}

IContentItem* IContentItem::itemFactory(const CItemType itemType,
		                                CRootItem* const root_item,
		                                CCategoryItem* const parent,
		                                const unsigned posInParent)
{
	const CItemType::item_type_t type_enum = itemType.getType();
	switch(type_enum) {
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
		return new CPlaylistItem(root_item, parent, posInParent);
		break;
	case CItemType::E_NEXTLISTITEM:
		return new CNextlistItem(root_item, parent, posInParent);
		break;
	// does CStreamClientItem a constructor without text ???
	default:
		return 0;
		break;
	}
}

IContentItem* IContentItem::itemFactory(const CItemType itemType,
		                                CRootItem* const root_item,
		                                std::string text,
		                                CCategoryItem* const parent,
		                                const unsigned posInParent) throw(ExMalformedPatch)
{
	const CItemType::item_type_t type_enum = itemType.getType();
	switch(type_enum) {
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
		return new CPlaylistItem(root_item, text, parent, posInParent);
		break;
	case CItemType::E_NEXTLISTITEM:
		return new CNextlistItem(root_item, text, parent, posInParent);
		break;
	case CItemType::E_STREAM_CLIENT:
		return new CStreamClientItem(root_item, text, parent, posInParent);
		break;
	default:
		return 0;
		break;
	}
}

IContentItem* IContentItem::itemFactory(CMediaItem* item,
		                                CRootItem *root_item,
		                                CCategoryItem *parent,
		                                const unsigned posInParent)
{
		 CMediaItem* newItem = new CMediaItem(root_item, parent, posInParent);
         newItem->setArtist(item->getArtist());
		 newItem->setAlbum(item->getAlbum());
		 newItem->setTitle(item->getTitle());
		 newItem->setDuration(item->getDuration());
		 newItem->setYear(item->getYear());
		 newItem->setFilename(item->getFilename());
		 return newItem;
}

IContentItem* IContentItem::itemFactory(const IContentItem& other,
                                               CRootItem* const root_item,
                                               CCategoryItem* const parent)
{
    const CItemType::item_type_t type_enum = other.type();
    switch(type_enum) {
    case CItemType::E_ROOT:
        return 0;
        break;
    case CItemType::E_CAT:
        return 0;
        break;
    case CItemType::E_MEDIAITEM:
    {
        const CMediaItem* const otherPtr = reinterpret_cast<const CMediaItem* const>(&other);
        CMediaItem* newItem = new CMediaItem(*otherPtr, root_item, parent);
        return newItem;
        break;
    }
    case CItemType::E_PLAYLISTITEM:
    {
        const CPlaylistItem* const otherPtr = reinterpret_cast<const CPlaylistItem* const>(&other);
        CPlaylistItem* newItem = new CPlaylistItem(*otherPtr, root_item, parent);
        return newItem;
        break;
    }
    case CItemType::E_NEXTLISTITEM:
    {
        const CNextlistItem* const otherPtr = reinterpret_cast<const CNextlistItem* const>(&other);
        CNextlistItem* newItem = new CNextlistItem(*otherPtr, root_item, parent);
        return newItem;
        break;
    }
    case CItemType::E_STREAM_CLIENT:
    {
        const CStreamClientItem* const otherPtr = reinterpret_cast<const CStreamClientItem* const>(&other);
        CStreamClientItem* newItem = new CStreamClientItem(*otherPtr, root_item, parent);
        return newItem;
        break;
    }
    default:
        return 0;
        break;
    }
}



bool IContentItem::operator==(const IContentItem& other) {
	return true;
}

uint32_t IContentItem::hash( std::string stdstr ) {
	uint32_t hash = 0;
	int c;
	const char* str = stdstr.c_str();

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

} // namespace muroa
