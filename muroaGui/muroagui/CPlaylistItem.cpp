/*
 * CPlaylistItem.cpp
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#include "CPlaylistItem.h"

CPlaylistItem::CPlaylistItem(QString itemStr) : CItemBase(itemStr) {
	m_key = itemStr;
}

CPlaylistItem::~CPlaylistItem() {
}

QString CPlaylistItem::getTitle(int col){
	return QString("Playlist Items");
}


QVariant CPlaylistItem::data(int column) const
{
	return QVariant(m_key);
}
