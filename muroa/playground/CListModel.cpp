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

#include "CListModel.h"

#include "CRootItem.h"
#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CPlaylistItem.h"
#include "CNextlistItem.h"

#include <iostream>
#include <stack>

using namespace std;


CListModel::CListModel() {
	m_model_base = getBase();
}

CListModel::~CListModel() {
	// TODO Auto-generated destructor stub
}

void CListModel::setBase(CCategoryItem* base) {
	m_model_base = base;
}


int CListModel::rowCount(const QModelIndex & index) const {
	return m_model_base->getNumContentItems();
}



QVariant CListModel::data(const QModelIndex & index, int role) const {
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}

	IContentItem* item = m_model_base->getContentItem( index.row() );
	if(!item)
		return QVariant();

	return item->getHash();

}



QVariant CListModel::headerData(int section, Qt::Orientation orientation, int role) {

}

