/*
 * CCollectionItem.cpp
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#include "CCollectionItem.h"
#include <QString>

CCollectionItem::CCollectionItem(QString itemStr) : CItemBase(itemStr)
{
	// TODO parse string here
	bool ok;
	// newItem.setFilename( itemStr.section(',', 0, 0) );
	setArtist( itemStr.section(',', 0, 0) );
	setAlbum( itemStr.section(',', 1, 1) );
	setYear( itemStr.section(',', 2, 2).toInt(&ok) );
	setTitle( itemStr.section(',', 3, 3) );
	setLengthInSec( itemStr.section(',', 4, 4).toInt(&ok) );

}

CCollectionItem::~CCollectionItem() {

}

QString CCollectionItem::getTitle(int col)
{
	QString title;
	switch(col)
	{
		case 0:
			title = "Artist";
			break;
		case 1:
			title = "Album";
			break;

		case 2:
			title = "Year";
			break;

		case 3:
			title = "Title";
			break;

		case 4:
			title = "Length";
			break;

		default:
			return QString("Hä?");

	}
	return title;
}
