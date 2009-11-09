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

class CCollection {
public:
	CCollection();
	virtual ~CCollection();

//	QString asText();

	inline void setText(QString collection, int revision)
	{
		m_collectionAsText = collection;
		m_revision = revision;
	};

	inline QString getText(void) { return m_collectionAsText; };
	inline int getRevision(void) { return m_revision; };

private:
	QList<CCollectionItem> m_items;

	void parse();


	QString m_collectionAsText;
	int m_revision;
};

#endif /* CCOLLECTION_H_ */
