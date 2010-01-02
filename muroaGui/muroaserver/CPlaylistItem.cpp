/*
 * CPlaylistItem.cpp
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#include "CPlaylistItem.h"

CPlaylistItem::CPlaylistItem() {
	// TODO Auto-generated constructor stub

}

CPlaylistItem::~CPlaylistItem() {
	// TODO Auto-generated destructor stub
}

void CPlaylistItem::setText(QString text)
{
	m_text = text;
	bool ok;
	m_collectionHash = m_text.toULong(&ok);
}


QString CPlaylistItem::assembleText()
{
	QString text(QString("%1\n").arg(m_collectionHash));

	return text;
}
