/***************************************************************************
 *
 *   CRenderClientsListModel.h
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
 *   along wi  th this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CRENDERCLIENTSLISTMODEL_H_
#define CRENDERCLIENTSLISTMODEL_H_

#include <QAbstractListModel>
#include <QIcon>
#include <CRootItem.h>
#include <CSession.h>
#include <IItemModel.h>
#include <CModelDiff.h>

class CTreeItem;
class CSession;
namespace muroa {
	class CItemBase;
	class CStreamClientItem;
}
class CRenderClientsListModel : public QAbstractListModel, public muroa::IItemModel {
public:
	CRenderClientsListModel(muroa::CRootItem* rootItem, const CSession* const session);
	virtual ~CRenderClientsListModel();

	void setRole( enum origin role) {m_role = role; };
    enum origin getRole() { return m_role; };

	void setBase(std::string path);
	inline muroa::CRootItem* getRootItem() const { return m_rootItem; };

	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole );

	bool beginInsertItems( const int pos, const int count, const muroa::CCategoryItem* parent );
	bool endInsertItems( );
	bool beginRemoveItems( const int pos, const int count, const muroa::CCategoryItem* parent );
	bool endRemoveItems( );

	muroa::CItemBase* itemFromIndex(QModelIndex index);

	int own2availIndex(int ownIndex);
	int avail2ownIndex(int availIndex);

private:

	// role based access functions
	int getNumSCItems() const;
    muroa::CStreamClientItem* getSCItem(int index) const;

    // the model's data is in here
	muroa::CRootItem* m_rootItem;
	bool m_deleteRootItem;

	QIcon m_enabled_client_icon;
	QIcon m_disabled_client_icon;

	muroa::CCategoryItem* m_model_base;

	const CSession* const m_session;

	std::string m_base_path;
	bool m_reset_base;

    enum origin m_role;

    void reset();
};

#endif /* CRENDERCLIENTSLISTMODEL_H_ */
