/***************************************************************************
 *
 *   CTreeModel.cpp
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

#include "CTreeModel.h"

#include "CRootItem.h"
#include "CCategoryItem.h"
#include "CMediaItem.h"


int CTreeModel::m_num_columns = 6;
QString CTreeModel::m_column_headers[] = {"Artist", "Album", "Title", "Year", "Duration", "Filename"};


CTreeModel::CTreeModel(CRootItem *rootItem) : m_rootItem(rootItem) {

}

CTreeModel::~CTreeModel() {

}

QVariant CTreeModel::data(const QModelIndex &index, int role) const {
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}

	CItemBase* item = itemFromIndex(index);
	if(!item)
		return QVariant();

	return dataFromColumn(item, index.column());
}

Qt::ItemFlags CTreeModel::flags(const QModelIndex &index) const {

}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}

	if(orientation == Qt::Horizontal)
	{
		return m_column_headers[section];
	}
	else
	{
		return QVariant();
	}

}

QModelIndex CTreeModel::index(int row, int column, const QModelIndex &parent) const {

}

QModelIndex CTreeModel::parent(const QModelIndex &index) const {

}

int CTreeModel::rowCount(const QModelIndex &parent) const {

}

int CTreeModel::columnCount(const QModelIndex &parent) const {
	return 6;
}

CItemBase* CTreeModel::itemFromIndex(const QModelIndex & index) const
{
	if (index.isValid()) {
		return static_cast<CItemBase *>(index.internalPointer());
	} else {
		return m_rootItem->getBase();
	}
}

QVariant CTreeModel::dataFromColumn(CItemBase* item, int column) const {

	QVariant result;
	switch(item->type()) {
		case E_CAT:
		{
			CCategoryItem *citem = reinterpret_cast<CCategoryItem*>(item);
			switch( column ) {
				case 0:
					result = QVariant(QString::fromUtf8( citem->getName().c_str() ));
					break;
				default:
					result = QVariant();
			}
			break;
		}

		case E_MEDIAITEM:
		{
			CMediaItem *mitem = reinterpret_cast<CMediaItem*>(item);
			switch( column ) {
				case 0:
					result = QVariant(QString::fromUtf8( mitem->getArtist().c_str() ));
					break;
				case 1:
					result = QVariant(QString::fromUtf8( mitem->getAlbum().c_str() ));
					break;
				case 2:
					result = QVariant(QString::fromUtf8( mitem->getTitle().c_str() ));
					break;
				case 3:
					result = mitem->getYear();
					break;
				case 4:
					result = mitem->getDuration();
					break;
				case 5:
					result = QVariant(QString::fromUtf8( mitem->getFilename().c_str() ));
					break;
				default:
					result = QVariant();
			}
			break;
		}
		default:
			break;
	}
	return result;
}

