/*
 * CCollectionItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CCollectionItem.h"
#include <QHash>

unsigned CCollectionItem::lastUsedHash = 0;


CCollectionItem::CCollectionItem(QString text) {
	CCollectionItem::lastUsedHash++;
//	m_hash = lastUsedHash;

	bool ok;

	m_filename = text.section(',', 0, 0);
	m_artist = text.section(',', 1, 1);
	m_album = text.section(',', 2, 2);
	m_year = text.section(',', 3, 3).toInt(&ok);
	m_title = text.section(',', 4, 4);
	m_length_in_s = text.section(',', 5, 5).toInt(&ok);

	m_hash = qHash(m_filename);
	assembleText();
}

CCollectionItem::~CCollectionItem() {
	// TODO Auto-generated destructor stub
}

//void CCollectionItem::setText(QString text)
//{
//	bool ok;
//	m_text = text;
//
//	m_filename = text.section(',', 0, 0);
//	m_artist = text.section(',', 1, 1);
//	m_album = text.section(',', 2, 2);
//	m_year = text.section(',', 3, 3).toInt(&ok);
//	m_title = text.section(',', 4, 4);
//	m_length_in_s = text.section(',', 5, 5).toInt(&ok);
//}

void CCollectionItem::setAlbum(QString album)
{
	m_album = album;
	assembleText();
}

void CCollectionItem::setArtist(QString artist)
{
	m_artist = artist;
	assembleText();
}

void CCollectionItem::setFilename(QString filename)
{
	m_filename = filename;
	assembleText();
}

void CCollectionItem::setLengthInSec(int lengthInSec)
{
	m_length_in_s = lengthInSec;
	assembleText();
}

void CCollectionItem::setTitle(QString title)
{
	m_title = title;
	assembleText();
}

void CCollectionItem::setYear(int year)
{
	m_year = year;
	assembleText();
}


void CCollectionItem::assembleText()
{
	m_text = QString("%1,%2,%3,%4,%5,%6,%7").arg(m_filename).arg(m_artist).arg(m_album).arg(m_year).arg(m_title).arg(m_length_in_s).arg(m_hash);
}
