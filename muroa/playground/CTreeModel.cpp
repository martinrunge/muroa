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

#include <iostream>

#include <stack>

using namespace std;

int CTreeModel::m_num_columns = 6;
QString CTreeModel::m_column_headers[] = {"Artist", "Album", "Title", "Year", "Duration", "Filename"};


CTreeModel::CTreeModel( )  {

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
	return QAbstractItemModel::flags(index);
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

	CItemBase *parentItem = itemFromIndex(parent);
	CItemBase *ptr = getBase();
	if(parentItem->numChildren() > 0 && parentItem->numChildren() > row)
	{
		ptr = parentItem->childAt(row);
	}

	return createIndex(row, column, ptr);
}

QModelIndex CTreeModel::parent(const QModelIndex &index) const {
	CItemBase *item = itemFromIndex(index);

	//qDebug() << QString("searching parent of: %1").arg(item->name());

	if(!item)
		return QModelIndex();
	CItemBase *parentItem = item->getParent();
	if (!parentItem)
		return QModelIndex();
	CItemBase *grandParentItem = parentItem->getParent();
	if (!grandParentItem)
		return QModelIndex();
	int row = grandParentItem->childPos(parentItem);
		return createIndex(row, 0, parentItem);
}

bool CTreeModel::hasChildren( const QModelIndex & parent ) {
	CItemBase *parentItem = itemFromIndex(parent);
	cout << "parent: (" << parent.row() << "," << parent.column() << ") ";
	if (!parentItem)
		return false;

	cout << "Type: " << parentItem->type() << " num children: " <<  parentItem->numChildren() << endl;

	return (parentItem->numChildren() > 0);
}


int CTreeModel::rowCount(const QModelIndex &parent) const {
	CItemBase *parentItem = itemFromIndex(parent);
	if (!parentItem)
		return 0;

	return parentItem->numChildren();
}

int CTreeModel::columnCount(const QModelIndex &parent) const {
	return 6;
}

bool CTreeModel::beginInsertItems( const int pos, const int count, const CCategoryItem* parent ) {
	QModelIndex parentIndex = index(parent);
    QAbstractItemModel::beginInsertRows(parentIndex, pos, pos + count - 1);
    return true;
}

bool CTreeModel::endInsertItems( ) {
	QAbstractItemModel::endInsertRows();
	return true;
}

bool CTreeModel::beginRemoveItems( const int pos, const int count, const CCategoryItem* parent ) {
	QModelIndex parentIndex = index(parent);
	QAbstractItemModel::beginRemoveRows(parentIndex, pos, pos + count - 1);
	return true;
}

bool CTreeModel::endRemoveItems( ) {
    QAbstractItemModel::endRemoveRows();
	return true;
}


CItemBase* CTreeModel::itemFromIndex(const QModelIndex & index) const
{
	if (index.isValid()) {
		return static_cast<CItemBase *>(index.internalPointer());
	} else {
		return getBase();
	}
}

QModelIndex CTreeModel::index(const CItemBase* item, int column) const {

	if(item ==  0) {
		return QModelIndex();
	}

	CCategoryItem* parent = item->getParent();
    if (parent == 0 ) {
        return QModelIndex();
    }

    unsigned posInParent = parent->childPos(item);

    return createIndex(posInParent, column, const_cast<CItemBase*>(item));
}



QModelIndex CTreeModel::indexFromItem(const CItemBase* item) const {
	CCategoryItem* parent = item->getParent();
	stack<CCategoryItem*> cat_stack;
	while(parent != 0) {
		cat_stack.push(parent);
		parent = parent->getParent();
	}

	QModelIndex mindex = QModelIndex();

	while(!cat_stack.empty() ) {
		CCategoryItem* citem = cat_stack.top();
		cat_stack.pop();
		if( citem != 0 ) {
			int row = parent->childPos(citem);
			mindex = index( row, 0, mindex);
		}
	}

	return mindex;
}



QVariant CTreeModel::dataFromColumn(CItemBase* item, int column) const {

	QVariant result;
	switch(item->type()) {
		case CItemType::E_CAT:
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

		case CItemType::E_MEDIAITEM:
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



