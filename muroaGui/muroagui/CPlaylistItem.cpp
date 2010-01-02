/*
 * CPlaylistItem.cpp
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#include "CPlaylistItem.h"
#include <QDebug>

#include <iostream>
using namespace std;

CPlaylistItem::CPlaylistItem(QString itemStr) : CItemBase(itemStr) {
	bool ok;
	m_hash = itemStr.toUInt(&ok);
	cerr << "CPlaylistItem: hash: " << dec << m_hash << " ptr: " << hex << this << endl;
}

CPlaylistItem::~CPlaylistItem() {
}

QString CPlaylistItem::getTitle(int col){
	return QString("Playlist Items");
}


QVariant CPlaylistItem::data(int column) const
{
	return QVariant(m_as_string);
}
