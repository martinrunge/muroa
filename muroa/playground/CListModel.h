/***************************************************************************
 *
 *   CListModel.h
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

#ifndef CLISTMODEL_H_
#define CLISTMODEL_H_

#include <QAbstractListModel>
#include "CRootItem.h"

class CTreeItem;
class CItemBase;

class CListModel : public QAbstractListModel, public CRootItem {
public:
	CListModel();
	virtual ~CListModel();

	void setBase(CCategoryItem* base);

	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole );

private:
	CCategoryItem* m_model_base;
};

#endif /* CLISTMODEL_H_ */
