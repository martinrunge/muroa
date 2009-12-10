/*
 * CCollectionModel.cpp
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#include "CCollectionModel.h"
#include "CCollection.h"

CCollectionModel::CCollectionModel(QObject* parent) : CModelBase<CCollectionItem>(parent)
{
}

CCollectionModel::~CCollectionModel()
{
}


int CCollectionModel::rowCount(const QModelIndex &parent) const
{
	return m_collectionPtr->size();
}

int CCollectionModel::columnCount(const QModelIndex &parent) const
{
	return CCollectionItem::getNumColumns();
}

void CCollectionModel::append(QList<CCollectionItem> newItems)
{
    beginInsertRows(QModelIndex(), m_collectionPtr->size(), m_collectionPtr->size() + newItems.size() - 1);

	m_collectionPtr->append(newItems);

    endInsertRows();
}

void CCollectionModel::append(CCollectionItem newItem)
{
    beginInsertRows(QModelIndex(), m_collectionPtr->size(), m_collectionPtr->size());

    m_collectionPtr->append(newItem);

    endInsertRows();
}


bool CCollectionModel::insertItem(CCollectionItem item, int pos)
{
    beginInsertRows(QModelIndex(), pos, pos);
	m_collectionPtr->insertItem(item, pos);
    endInsertRows();
}

bool CCollectionModel::removeItem(int pos)
{
    beginRemoveRows(QModelIndex(), pos, pos);
	m_collectionPtr->removeItem(pos);
	endRemoveRows();
}


bool CCollectionModel::insertRows(int row, int count, const QModelIndex & parent)
{

    beginInsertRows(parent, row, row + count - 1);

    for (int i=0; i < count; i++) {
    	// m_collectionPtr->insert(row, content);
    }

    endInsertRows();
    return true;

}

bool CCollectionModel::removeRows(int row, int count, const QModelIndex & parent)
{

}


QVariant CCollectionModel::data(const QModelIndex &index, int role) const
{
	if(role !=Qt::DisplayRole)
	{
		return QVariant();
	}
	return m_collectionPtr->data(index.row(), index.column());
}

QVariant CCollectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role !=Qt::DisplayRole)
	{
		return QVariant();
	}
	if( orientation == Qt::Horizontal )
	{
		return CCollectionItem::getTitle(section);
		//return m_collectionPtr->getTitle(section);
	}
	else
	{
		return QVariant();
	}

}


QString CCollectionModel::getItemAsString(int pos)
{
	return m_collectionPtr->getItemAsString(pos);
}
