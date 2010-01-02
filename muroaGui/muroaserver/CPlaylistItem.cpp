/*
 * CPlaylistItem.cpp
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#include "CPlaylistItem.h"

CPlaylistItem::CPlaylistItem(QString text) {
	bool ok;
	m_collectionHash = text.toUInt(&ok);
	assembleText();
}

CPlaylistItem::~CPlaylistItem() {
	// TODO Auto-generated destructor stub
}

//void CPlaylistItem::setText(QString text)
//{
//	m_text = text;
//	bool ok;
//	m_collectionHash = m_text.toULong(&ok);
//}


void CPlaylistItem::assembleText()
{
	QString text(QString("%1").arg(m_collectionHash));
	m_text = text;
}
