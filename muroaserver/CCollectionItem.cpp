/*
 * CCollectionItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CCollectionItem.h"

#include <sstream>
#include <functional>

using namespace std;

CCollectionItem::CCollectionItem() {
}


CCollectionItem::CCollectionItem(QString text) {
	bool ok;

	m_filename = text.section(',', 0, 0);
	m_artist = text.section(',', 1, 1);
	m_album = text.section(',', 2, 2);
	m_year = text.section(',', 3, 3).toInt(&ok);
	m_title = text.section(',', 4, 4);
	m_duration_in_s = text.section(',', 5, 5).toInt(&ok);

	assembleText();
}

CCollectionItem::~CCollectionItem() {
	// TODO Auto-generated destructor stub
}

void CCollectionItem::setAlbum(QString album)
{
	m_album = album;
	assembleText();
}

void CCollectionItem::setAlbum(std::string album) {
	setAlbum(QString::fromUtf8(album.c_str(), album.size()));
}


void CCollectionItem::setArtist(QString artist)
{
	m_artist = artist;
	assembleText();
}

void CCollectionItem::setArtist(std::string artist) {
	setArtist(QString::fromUtf8(artist.c_str(), artist.size()));
}


void CCollectionItem::setFilename(QString filename)
{
	m_filename = filename;
	assembleText();
}

void CCollectionItem::setFilename(std::string filename)
{
	setFilename(QString::fromUtf8(filename.c_str(), filename.size()));
}

void CCollectionItem::setDuration(int duration)
{
	m_duration_in_s = duration;
	assembleText();
}

void CCollectionItem::setTitle(QString title)
{
	m_title = title;
	assembleText();
}

void CCollectionItem::setTitle(std::string title)
{
	setTitle( QString::fromUtf8( title.c_str(), title.size() ) );
}

void CCollectionItem::setYear(int year)
{
	m_year = year;
	assembleText();
}


void CCollectionItem::assembleText()
{
//	  m_text = QString("%1,%2,%3,%4,%5,%6").arg(m_filename).arg(m_artist).arg(m_album).arg(m_year).arg(m_title).arg(m_duration_in_s);
//    m_hash = qHash(m_text);
//    m_text.append(QChar(','));
//    m_text.append(QString::number(m_hash, 10));

	stringstream ss;

	ss << m_filename.toUtf8().constData() << ","
	   << m_artist.toUtf8().constData() << ","
	   << m_album.toUtf8().constData() << ","
	   << m_title.toUtf8().constData() << ","
	   << m_year << ","
	   << m_duration_in_s;

	m_hash = hash<string>()( ss.str() );
	ss << "," << m_hash;

	m_text = QString::fromUtf8( ss.str().c_str(), ss.str().size() );

}

