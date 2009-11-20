/*
 * CCollectionItem.cpp
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#include "CCollectionItem.h"
#include <QString>

CCollectionItem::CCollectionItem(QString itemStr) : m_as_string(itemStr)
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
