/*
 * CCollection.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CCollection.h"

CCollection::CCollection() {
	// TODO Auto-generated constructor stub

}

CCollection::~CCollection() {
	// TODO Auto-generated destructor stub
}


void CCollection::parseLine(QString line)
{
	CCollectionItem newItem;
	// line is supposed to be comma seperated

	bool ok;
	newItem.setFilename( line.section(',', 0, 0) );
	newItem.setArtist( line.section(',', 1, 1) );
	newItem.setAlbum( line.section(',', 2, 2) );
	newItem.setYear( line.section(',', 3, 3).toInt(&ok) );
	newItem.setTitle( line.section(',', 4, 4) );
	newItem.setLengthInSec( line.section(',', 5, 5).toInt(&ok) );

	m_items.append(newItem);
}

QString CCollection::asText()
{
	QString collection;

	for(int i=0; i < m_items.size(); i++)
	{
		CCollectionItem it = m_items.at(i);
		QString line = QString("%1,%2,%3,%4,%5\n").arg(it.getArtist()).arg(it.getAlbum()).arg(it.getYear()).arg(it.getTitle()).arg(it.getLengthInSec());
		collection.append(line);
	}

	return collection;
}

