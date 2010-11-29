/*
 * CModelBase.h
 *
 *  Created on: 5 Dec 2009
 *      Author: martin
 */

#ifndef CMODELBASE_H_
#define CMODELBASE_H_

#include <QAbstractTableModel>
#include <QVariant>
#include <QString>

#include "CCollection.h"


template <class T>
class CModelBase : public QAbstractTableModel {
public:
	CModelBase(QObject* parent = 0) : QAbstractTableModel(parent) {};
	virtual ~CModelBase() {};

	inline void setCollection(CCollection<T>* collectionPtr) { m_collectionPtr = collectionPtr; };

	virtual int rowCount(const QModelIndex &parent) const = 0;
	virtual int columnCount(const QModelIndex &parent) const = 0;

	virtual void append(QList<T> newItems) = 0;
	virtual void append(T newItems) = 0;

	virtual bool insertItem(T item, int pos) = 0;
	virtual bool removeItem(int pos) = 0;

	virtual QVariant data(const QModelIndex &index, int role) const = 0;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const = 0;

	virtual T itemAt(int pos) const = 0;
	virtual QString getItemAsString(int pos) = 0;
private:
	CCollection<T>* m_collectionPtr;

};

// template <class T>




#endif /* CMODELBASE_H_ */
