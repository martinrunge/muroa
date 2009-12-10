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


template <class T>
class CCollection {
public:
	CCollection() {};
	virtual ~CCollection() {};

	inline int size() { return m_items.size(); };

	inline void append(QList<T> newItems) { m_items.append(newItems); };
	inline void append(T newItem) { m_items.append(newItem); };

	QString data(int row, int column);
	QString asString();
	inline QString getItemAsString(int pos) { return m_items.at(pos).asString(); };
	inline void insertItem(T item, int pos) { m_items.insert(pos, item); };
	void insertItems(QList<T> items, int pos);
	inline void removeItem(int pos) { m_items.removeAt(pos); };
	void removeItems(int pos, int num);

	//inline int getNumColumns() {return 5; };
	//QString getTitle(int col);

private:
	QList<T> m_items;
};



template <class T> QString CCollection<T>::data(int row, int column)
{
	QVariant data;

	if(row > size() || column > 5)
	{
		return QString();
	}

	const T &item = m_items.at(row);
	data = item.data(column);
	return data.toString();

}


template <class T> QString CCollection<T>::asString()
{
	QString text;
	for(int i = 0; i < m_items.size(); i++)
	{
		text.append(m_items.at(i).asString());
		text.append("\n");
	}
	return text;
}



template <class T> void CCollection<T>::insertItems(QList<T> items, int pos)
{
	// TODO clever handling of multiple inserts
	for(int i = 0; i < items.size(); i++)
	{
		m_items.insert(pos + i, items.at(i));
	}
}


template <class T> void CCollection<T>::removeItems(int pos, int num)
{
	// TODO clever handling of multiple removes
	for(int i = 0; i < num; i++)
	{
		m_items.removeAt(pos + i);
	}
}




#endif /* CCOLLECTION_H_ */
