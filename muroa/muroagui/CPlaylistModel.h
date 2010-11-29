/*
 * CPlaylistModel.h
 *
 *  Created on: 1 Dec 2009
 *      Author: martin
 */

#ifndef CPLAYLISTMODEL_H_
#define CPLAYLISTMODEL_H_


#include <QVariant>
#include <QList>
#include <QStringList>

#include "CModelBase.h"
#include "CCollection.h"
#include "CPlaylistItem.h"
#include "CPlaylistCommand.h"



class CPlaylistModel : public CModelBase<CPlaylistItem*> {
	Q_OBJECT;
public:
	CPlaylistModel(QObject* parent = 0);
	virtual ~CPlaylistModel();

	inline void setPlaylist(CCollection<CPlaylistItem*>* playlistPtr) { m_playlistPtr = playlistPtr; };
	inline void setCollections(CCollection<CPlaylistItem*>* playlistPtr, CCollection<CCollectionItem*>* collectionPtr)
	{
		m_playlistPtr = playlistPtr;
		m_collectionPtr = collectionPtr;
	};

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	void append(QList<CPlaylistItem*> newItems);
	void append(CPlaylistItem* newItems);

	bool insertItem(CPlaylistItem* item, int pos);
	bool removeItem(int pos);

	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant & value, int role = Qt::EditRole );
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	inline Qt::ItemFlags flags( const QModelIndex & index ) const
	{
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	}

	QStringList mimeTypes() const;

	Qt::DropActions supportedDropActions () const
	{
		return Qt::MoveAction;
	}

	QString getItemAsString(int pos);

	inline  CPlaylistItem* itemAt(int pos) const
	{
		return m_playlistPtr->at(pos);
	}

	//void makeDiff(CModelDiff* diff);

signals:
	void sendCommand(const CCommandBase& cmd);

private:
	CCollection<CPlaylistItem*>* m_playlistPtr;
	CCollection<CCollectionItem*>* m_collectionPtr;
};

#endif /* CPLAYLISTMODEL_H_ */
