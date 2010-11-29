/*
 * CCollectionModel.h
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTIONMODEL_H_
#define CCOLLECTIONMODEL_H_

#include <QAbstractTableModel>
#include <QVariant>

#include "CModelBase.h"
#include "CCollection.h"
#include "CCollectionItem.h"
#include "CCommandBase.h"


class CCollectionModel : public CModelBase<CCollectionItem*> {
	Q_OBJECT;
public:
	CCollectionModel(QObject* parent = 0);
	virtual ~CCollectionModel();

	inline void setCollection(CCollection<CCollectionItem*>* collectionPtr) { m_collectionPtr = collectionPtr; };

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	void append(QList<CCollectionItem*> newItems);
	void append(CCollectionItem* newItems);

	bool insertItem(CCollectionItem* item, int pos);
	bool removeItem(int pos);

	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	inline Qt::ItemFlags flags( const QModelIndex & index ) const
	{
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	}

	QStringList mimeTypes() const;

	//void makeDiff(CModelDiff* diff);

	CCollectionItem* itemAt(int pos) const;
	QString getItemAsString(int pos);

signals:
	void sendCommand(const CCommandBase& cmd);

private:
	CCollection<CCollectionItem*>* m_collectionPtr;

};

#endif /* CCOLLECTIONMODEL_H_ */
