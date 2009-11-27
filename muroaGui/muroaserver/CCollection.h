/*
 * CCollection.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTION_H_
#define CCOLLECTION_H_

#include <QList>
#include "CCollectionItem.h"

template <class T>
class CCollection
{
public:
	CCollection() {} ;
	virtual ~CCollection() {} ;

	inline void setText(QString collection, int revision)
	{
		m_collectionAsText = collection;
		m_revision = revision;
	};

	inline QString getText(void);
	inline int getRevision(void) { return m_revision; };

private:
	QList<T> m_items;

	void parse();


	QString m_collectionAsText;
	int m_revision;
};



template <class T> void CCollection<T>::parse()
{
	int start(0);
	int end(-1);

	int index;
	while( (index = m_collectionAsText.indexOf(QChar::LineSeparator, start)) != -1 )
	{
		QStringRef line(&m_collectionAsText, start, index - start);
		CCollectionItem newItem;
		newItem.setText(*line.string());
		m_items.append(newItem);
	}
}

template <class T> QString CCollection<T>::getText()
{
	QString collection;

	for(int i=0; i < m_items.size(); i++)
	{
		QString line = m_items.at(i).getText();
		collection.append(line);
	}

	return collection;
}




#endif /* CCOLLECTION_H_ */
