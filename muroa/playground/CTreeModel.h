/***************************************************************************
 *
 *   CTreeModel.h
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

#ifndef CTREEMODEL_H_
#define CTREEMODEL_H_

#include <QAbstractItemModel>


class CTreeItem;
class CRootItem;
class CItemBase;

class CTreeModel: public QAbstractItemModel {
	Q_OBJECT;
public:
	CTreeModel(CRootItem* muroaRootItem);
	virtual ~CTreeModel();

	QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    QVariant dataFromColumn(CItemBase* item, int column) const;
    CItemBase* itemFromIndex(const QModelIndex & index) const;

    CRootItem *m_rootItem;

    static int m_num_columns;
    static QString m_column_headers[];
};

#endif /* CTREEMODEL_H_ */
