/*
 * CPlaylistItem.cpp
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#include "CPlaylistItem.h"

CPlaylistItem::CPlaylistItem(QString itemStr) : CItemBase(itemStr) {

}

CPlaylistItem::~CPlaylistItem() {
}

QString CPlaylistItem::getTitle(int col){
	return QString("Playlist Items");
}

