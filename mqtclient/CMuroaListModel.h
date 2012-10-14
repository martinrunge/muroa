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
#include <CRootItem.h>
#include <IItemModel.h>

class CTreeItem;
namespace muroa {
	class CItemBase;
}
class CMuroaListModel : public QAbstractListModel, public muroa::IItemModel {
public:
	CMuroaListModel(muroa::CRootItem* rootItem = 0);
	virtual ~CMuroaListModel();

	void setBase(std::string path);
	inline muroa::CRootItem* getRootItem() const { return m_rootItem; };


	inline void setMediaCol(muroa::CRootItem* ri) { m_mediaCol = ri; };
	inline muroa::CRootItem* getMediaCol() { return m_mediaCol; };
	inline void setPlaylist(muroa::CRootItem* ri) { m_playlist = ri; };
	inline muroa::CRootItem* getPlaylist() { return m_playlist; };

	int rowCount(const QModelIndex& index) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole );

	bool beginInsertItems( const int pos, const int count, const muroa::CCategoryItem* parent );
	bool endInsertItems( );
	bool beginRemoveItems( const int pos, const int count, const muroa::CCategoryItem* parent );
	bool endRemoveItems( );

	muroa::CItemBase* itemFromIndex(QModelIndex index);

private:

	// the model's data is in here
	muroa::CRootItem* m_rootItem;
	bool m_deleteRootItem;

	muroa::CCategoryItem* m_model_base;

	muroa::CRootItem* m_mediaCol;
	muroa::CRootItem* m_playlist;

	std::string m_base_path;
	bool m_reset_base;

    void reset();
};

#endif /* CLISTMODEL_H_ */
