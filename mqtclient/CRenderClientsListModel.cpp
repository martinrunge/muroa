/***************************************************************************
 *
 *   CRenderClientsListModel.cpp
 *
 *
 *   This file is part of the muroa multi room media player                *
 *
 *      http://www.muroa.org                                               *
 *
*   Copyright (C) 2012 by Martin Runge <martin.runge@web.de>              *
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

#include "CRenderClientsListModel.h"

#include "CRootItem.h"
#include "CSession.h"
#include "CCategoryItem.h"
#include "CStreamClientItem.h"

#include <iostream>
#include <stack>

using namespace std;
using namespace muroa;


CRenderClientsListModel::CRenderClientsListModel(muroa::CRootItem* rootItem, const CSession* const session):
		                                               m_rootItem(rootItem),
		                                               m_session(session),
		                                               m_deleteRootItem(false),
		                                               m_reset_base(false),
		                                           	   m_enabled_client_icon(":/icons/icons/network-connect.png"),
		                                           	   m_disabled_client_icon(":/icons/icons/network-disconnect.png")
{
	if(rootItem == 0) {
		m_rootItem = new CRootItem();
		m_deleteRootItem = true;
	}
	m_model_base = m_rootItem->getBase();
	m_rootItem->connectItemModel(this);
}

CRenderClientsListModel::~CRenderClientsListModel() {
	if(m_deleteRootItem) {
		delete m_rootItem;
	}
}

void CRenderClientsListModel::setBase(string path) {
	m_base_path = path;

	CCategoryItem* baseCatItem = m_rootItem->getCategoryPtr(m_base_path);
	if(baseCatItem == 0) {
		// not yet there, create it
		baseCatItem = m_rootItem->mkPath(m_base_path);
	}
	m_model_base = baseCatItem;
}


int CRenderClientsListModel::rowCount(const QModelIndex & index) const {
    int num = getNumSCItems();
	return num;
}


QVariant CRenderClientsListModel::data(const QModelIndex & index, int role) const {
    CStreamClientItem* scItem = getSCItem( index.row() );
    if(!scItem) {
        return QVariant();
    }
    switch(role)
    {
        case Qt::ToolTipRole:
	    {
	        QString entry = QString("owned by session: %1").arg(scItem->getOwnerSessionName().c_str());
	        return entry;
	    }
        break;
	    case Qt::DisplayRole:
	    {
	        if(m_role == E_OWN_RENDER_CLIENT)
	        {
	            if(scItem->getOwnerSessionName().compare(m_session->getName()) == 0) {
	                QString entry = QString("%1 [%2]").arg(scItem->getServiceName().c_str()).arg(scItem->getHostName().c_str());
	                return entry;
	            }
	        }
	        QString entry = QString("%1 [%2]").arg(scItem->getServiceName().c_str()).arg(scItem->getHostName().c_str());
	        return entry;
	    }
	    break;
	    case Qt::DecorationRole:
	    {
	        if(scItem->isEnabled()) {
	            return m_enabled_client_icon;
	        } else {
	            return m_disabled_client_icon;
	        }
	    }
	    break;
	    default:
	        return QVariant();
	        break;
	}
}

CItemBase* CRenderClientsListModel::itemFromIndex(QModelIndex index) {
	CItemBase* item = m_model_base->getContentItem( index.row() );
	return item;
}

QVariant CRenderClientsListModel::headerData(int section, Qt::Orientation orientation, int role) {
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}
	return QString("Playlist");
}

bool CRenderClientsListModel::beginInsertItems( const int pos, const int count, const CCategoryItem* parent ) {
	QAbstractListModel::beginInsertRows( QModelIndex(), pos, pos + count - 1);
    return true;
}

bool CRenderClientsListModel::endInsertItems( ) {
	QAbstractListModel::endInsertRows();
	return true;
}

bool CRenderClientsListModel::beginRemoveItems( const int pos, const int count, const CCategoryItem* parent ) {
	if(parent == m_rootItem->getBase()) {
		//deleteing all -> keep m_base
		m_reset_base = true;
	} else {
		m_reset_base = false;
	}

	QAbstractListModel::beginRemoveRows(QModelIndex(), pos, pos + count - 1);
	return true;
}

bool CRenderClientsListModel::endRemoveItems( ) {
	QAbstractListModel::endRemoveRows();
	if(m_reset_base) {
		setBase(m_base_path);
	}
	return true;
}

void CRenderClientsListModel::reset()
{
	m_rootItem->reset();
	setBase("/");
}

// role based access functions
int CRenderClientsListModel::getNumSCItems() const {
    int numItems = m_model_base->getNumContentItems();
    if(m_role == E_AVAIL_RENDER_CLIENT) {
        return numItems;
    }
    int numOwnClients = 0;
    for(int i = 0; i < numItems; i++) {
        IContentItem* ci = m_model_base->getContentItem(i);
        if(ci->type() == CItemType::E_STREAM_CLIENT ) {
            CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
            if(m_session->getName().compare( sci->getOwnerSessionName() ) == 0 ) {
                // this item blongs to this session
                numOwnClients++;
            }
        }
    }
    return numOwnClients;
}

muroa::CStreamClientItem* CRenderClientsListModel::getSCItem(int index) const {
    int numItems = m_model_base->getNumContentItems();
    if(m_role == E_AVAIL_RENDER_CLIENT) {
        IContentItem* ci = m_model_base->getContentItem(index);
        assert(ci->type() == CItemType::E_STREAM_CLIENT);
        CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
        return sci;
    }
    else
    {
        int ownIndex = 0;
        for(int i = 0; i < numItems; i++) {
            IContentItem* ci = m_model_base->getContentItem(i);
            if(ci->type() == CItemType::E_STREAM_CLIENT ) {
                CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);
                if(m_session->getName().compare( sci->getOwnerSessionName() ) == 0 ) {
                    // this item blongs to this session
                    if(ownIndex == index) {
                        return sci;
                    }
                    ownIndex++;
                }
            }
        }
        return 0;
    }
}





