/*
 * CPlaylistModel.cpp
 *
 *  Created on: 1 Dec 2009
 *      Author: martin
 */

#include "CPlaylistModel.h"


CPlaylistModel::CPlaylistModel(QObject* parent) : CModelBase<CPlaylistItem*>(parent),
												  m_collectionPtr(0),
												  m_playlistPtr(0)
{

}

CPlaylistModel::~CPlaylistModel() {
}



int CPlaylistModel::rowCount(const QModelIndex &parent) const
{
	return m_playlistPtr->size();
}

int CPlaylistModel::columnCount(const QModelIndex &parent) const
{
	return CPlaylistItem::getNumColumns();
}

void CPlaylistModel::append(QList<CPlaylistItem*> newItems)
{
    beginInsertRows(QModelIndex(), m_playlistPtr->size(), m_playlistPtr->size() + newItems.size() - 1);

	m_playlistPtr->append(newItems);

    endInsertRows();
}

void CPlaylistModel::append(CPlaylistItem* newItem)
{
    beginInsertRows(QModelIndex(), m_playlistPtr->size(), m_playlistPtr->size());

    m_playlistPtr->append(newItem);

    endInsertRows();
}


bool CPlaylistModel::insertItem(CPlaylistItem* item, int pos)
{
    beginInsertRows(QModelIndex(), pos, pos);
	m_playlistPtr->insertItem(item, pos);
    endInsertRows();
}

bool CPlaylistModel::removeItem(int pos)
{
    beginRemoveRows(QModelIndex(), pos, pos);
	m_playlistPtr->removeItem(pos);
	endRemoveRows();
}


bool CPlaylistModel::insertRows(int row, int count, const QModelIndex & parent)
{

    beginInsertRows(parent, row, row + count - 1);

    for (int i=0; i < count; i++) {
    	m_playlistPtr->insertItem(new CPlaylistItem(""), row + i);
    }

    endInsertRows();
    return true;

}

bool CPlaylistModel::removeRows(int row, int count, const QModelIndex & parent)
{
	beginRemoveRows(parent, row, count);
    m_playlistPtr->removeItems(row, count);
    endRemoveRows();

    return true;
}


QVariant CPlaylistModel::data(const QModelIndex &index, int role) const
{
	if(role !=Qt::DisplayRole)
	{
		return QVariant();
	}
	int row = index.row();
	unsigned hash = m_playlistPtr->hash(row);
	CCollectionItem* item = m_collectionPtr->getByHash(hash);

	// return m_playlistPtr->data(index.row(), index.column());
	//if(!item) return QVariant();
	if(!item)
	{
		m_collectionPtr->dump();
		return QVariant();
	}

	QString playlistentry = QString("%1  %2").arg(item->data(0).toString()).arg(item->data(3).toString());

	return playlistentry;
}

bool CPlaylistModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
	int row = index.row();
	if( row >= 0 && row < m_playlistPtr->size() && ( role == Qt::EditRole || role == Qt::DisplayRole ))
	{
		m_playlistPtr->setData(index.row(), index.column(), value);
		emit dataChanged(index, index);

	}


	return false;
}

QVariant CPlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role !=Qt::DisplayRole)
	{
		return QVariant();
	}
	if( orientation == Qt::Horizontal )
	{
		return CPlaylistItem::getTitle(section);
		//return m_playlistPtr->getTitle(section);
	}
	else
	{
		return QVariant();
	}

}


QString CPlaylistModel::getItemAsString(int pos)
{
	return m_playlistPtr->getItemAsString(pos);
}

QStringList CPlaylistModel::mimeTypes() const
{
	QStringList sl;

	sl << "application/x-muroa-playliust-diff";

	return sl;
}


void CPlaylistModel::makeDiff(CModelDiff* diff)
{
	QString diffStr = m_playlistPtr->diff(diff);

    CPlaylistCommand plCmd(diffStr);
    emit sendCommand(plCmd);

 }
