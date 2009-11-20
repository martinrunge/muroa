/*
 * CCollection.h
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTION_H_
#define CCOLLECTION_H_

#include <QList>
#include <QString>
#include "CCollectionItem.h"


class CCollection {
public:
	CCollection();
	virtual ~CCollection();

	int size();

	void append(QList<CCollectionItem> newItems);
	void append(CCollectionItem newItems);

	QString data(int row, int column);
	QString asString();
	QString getItemAsString(int pos);
	void insertItem(CCollectionItem item, int pos);
	void insertItems(QList<CCollectionItem> items, int pos);
	void removeItem(int pos);
	void removeItems(int pos, int num);

	inline int getNumColumns() {return 5; };
	QString getTitle(int col);

private:
	QList<CCollectionItem> m_items;

};

#endif /* CCOLLECTION_H_ */
