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

	m_hash = itemStr.section(',', 4, 4).toULong(&ok);

}

CCollectionItem::~CCollectionItem() {

}

QVariant CCollectionItem::data(int column) const
{
	QVariant data;
	switch(column)
	{
		case 0:
			data = m_artist;
			break;
		case 1:
			data = m_album;
			break;

		case 2:
			data = m_year;
			break;

		case 3:
			data = m_title;
			break;

		case 4:
		{
			data = QString("%1:%2").arg(m_length_in_s / 60).arg(m_length_in_s % 60);
			break;
		}
		default:
			return QString("Hä?");
	}
	return data;

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
