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

	void parseLine(QString line);
	QString asText();



private:
	QList<CCollectionItem> m_items;
};

#endif /* CCOLLECTION_H_ */
