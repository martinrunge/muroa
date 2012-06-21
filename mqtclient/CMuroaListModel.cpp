/***************************************************************************
 *
 *   CListModel.cpp
 *
 *   This file is part of playground                                  *
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

#include "CMuroaListModel.h"

#include "CRootItem.h"
#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CPlaylistItem.h"
#include "CNextlistItem.h"

#include <iostream>
#include <stack>

using namespace std;


CMuroaListModel::CMuroaListModel(): m_mediaCol(0), m_playlist(0) {
	m_model_base = getBase();
}

CMuroaListModel::~CMuroaListModel() {
	// TODO Auto-generated destructor stub
}

void CMuroaListModel::setBase(CCategoryItem* base) {
	m_model_base = base;
}


int CMuroaListModel::rowCount(const QModelIndex & index) const {
	return m_model_base->getNumContentItems();
}



QVariant CMuroaListModel::data(const QModelIndex & index, int role) const {
	if(role == Qt::ToolTipRole)
	{
		IContentItem* item = m_model_base->getContentItem( index.row() );
		if(!item)
			return QVariant();
		if(item->type() ==  CItemType::E_PLAYLISTITEM ) {
			CPlaylistItem* plItem = reinterpret_cast<CPlaylistItem*>(item);
			return QString("Playlist ItemID: %1\n"
					        "points to MediaItem Hash: %2").arg(plItem->getHash()).arg(plItem->getMediaItemHash());
		}
		else {
			CNextlistItem* nlItem = reinterpret_cast<CNextlistItem*>(item);
			return QString("Nextlist ItemID: %1 \n "
					        "points to playlist item -> %2\n"
					        "mediaItem hash: %3").arg(nlItem->getHash()).arg(nlItem->getPlaylistItemHash()).arg(nlItem->getMediaItemHash());
		}
	}
	else if(role == Qt::DisplayRole)
	{
		CMediaItem* mItem(0);
		IContentItem* ci(0);
		IContentItem* item = m_model_base->getContentItem( index.row() );
		if(!item || m_mediaCol == 0) {
			return QVariant();
		}
		if(item->type() ==  CItemType::E_PLAYLISTITEM ) {
			CPlaylistItem* plItem = reinterpret_cast<CPlaylistItem*>(item);
			ci = m_mediaCol->getContentPtr(CItemType(CItemType::E_MEDIAITEM), plItem->getMediaItemHash() );
		}
		else {
			CNextlistItem* nlItem = reinterpret_cast<CNextlistItem*>(item);
			ci = m_mediaCol->getContentPtr(CItemType(CItemType::E_MEDIAITEM), nlItem->getMediaItemHash() );
		}
		if(ci == 0) {
			return QVariant();
		}
		mItem = reinterpret_cast<CMediaItem*>(ci);
		QString entry = QString("%1 %2").arg(QString::fromUtf8(mItem->getArtist().c_str()))
				                        .arg(QString::fromUtf8(mItem->getTitle().c_str()));
		return entry;
	}
	else
	{
		return QVariant();
	}
}

CItemBase* CMuroaListModel::itemFromIndex(QModelIndex index) {
	CItemBase* item = m_model_base->getContentItem( index.row() );
	return item;
}



QVariant CMuroaListModel::headerData(int section, Qt::Orientation orientation, int role) {
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}
	return QString("Playlist");
}

bool CMuroaListModel::beginInsertItems( const int pos, const int count, const CCategoryItem* parent ) {
	QAbstractListModel::beginInsertRows( QModelIndex(), pos, pos + count - 1);
    return true;
}

bool CMuroaListModel::endInsertItems( ) {
	QAbstractListModel::endInsertRows();
	return true;
}

bool CMuroaListModel::beginRemoveItems( const int pos, const int count, const CCategoryItem* parent ) {
	QAbstractListModel::beginRemoveRows(QModelIndex(), pos, pos + count - 1);
	return true;
}

bool CMuroaListModel::endRemoveItems( ) {
	QAbstractListModel::endRemoveRows();
	return true;
}

void CMuroaListModel::init()
{
	CRootItem::init();
}

void CMuroaListModel::clear()
{
	beginResetModel();
	CRootItem::clear();
	setBase(CRootItem::getBase());
	endResetModel();
}




