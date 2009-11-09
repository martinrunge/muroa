/*
 * CCollectionItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CCollectionItem.h"

CCollectionItem::CCollectionItem() {
	// TODO Auto-generated constructor stub

}

CCollectionItem::~CCollectionItem() {
	// TODO Auto-generated destructor stub
}

void CCollectionItem::setStringRef(QStringRef stringRef)
{
	bool ok;
	m_filename = stringRef.string()->section(',', 0, 0);
	m_artist = stringRef.string()->section(',', 1, 1);
	m_album = stringRef.string()->section(',', 2, 2);
	m_year = stringRef.string()->section(',', 3, 3).toInt(&ok);
	m_title = stringRef.string()->section(',', 4, 4);
	m_length_in_s = stringRef.string()->section(',', 5, 5).toInt(&ok);
}
