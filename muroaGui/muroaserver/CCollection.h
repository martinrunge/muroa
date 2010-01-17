/*
 * CCollection.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTION_H_
#define CCOLLECTION_H_

#include <QList>
#include <QString>
#include "CCollectionItem.h"
#include <QDebug>

template <class T>
class CCollection
{
public:
	CCollection() {} ;
	CCollection(const CCollection<T>& other);
	virtual ~CCollection() {} ;

	inline void setText(QString collection, int revision)
	{
		m_collectionAsText = collection;
		m_revision = revision;
		parse();
	};

	int patch(const QString& diff, int revision);

	inline int size() const { return m_items.size(); };
	inline T getItem(int pos) const { return m_items.at(pos); };

	QString getText(void);
	inline int getRevision(void) { return m_revision; };

private:
	QList<T> m_items;

	void parse();

	QString m_collectionAsText;
	int m_revision;
};



template <class T> CCollection<T>::CCollection(const CCollection<T>& other)
{
	m_items = other.m_items;
	m_collectionAsText = other.m_collectionAsText;
	m_revision = other.m_revision;
}

template <class T> void CCollection<T>::parse()
{
	int start(0);
	int end(-1);

	int index;
	//while( (index = m_collectionAsText.indexOf(QChar::LineSeparator, start)) != -1 )
	while( (index = m_collectionAsText.indexOf('\n', start)) != -1 )
	{
		QStringRef line(&m_collectionAsText, start, index - start);
		qDebug() << QString("parsed line: %1") .arg( line.toString());
		T newItem(line.toString());
		m_items.append(newItem);
		start = index + 1;
	}
}

template <class T> QString CCollection<T>::getText()
{
	QString collection;
	qDebug() << QString("CCollection<T>::getText(): m_items.size() = %1").arg(m_items.size());
	for(int i=0; i < m_items.size(); i++)
	{
		QString line = m_items.at(i).getText();
		qDebug() << QString("appending %1").arg(line);
		collection.append(line).append('\n');
	}

	return collection;
}

template <class T> int CCollection<T>::patch(const QString& diff, int revision)
{
	qDebug() << QString("CCollection<T>::patch: %1").arg(diff);
}


#endif /* CCOLLECTION_H_ */
