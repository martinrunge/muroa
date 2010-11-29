/*
 * CMediaItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CMediaItem.h"

using namespace std;

CMediaItem::CMediaItem() {
}

CMediaItem::~CMediaItem() {
}

void CMediaItem::setAlbum(string album)
{
	m_album = album;
}

void CMediaItem::setArtist(string artist)
{
	m_artist = artist;
}

void CMediaItem::setFilename(string filename)
{
	m_filename = filename;
}

void CMediaItem::setDuration(int duration)
{
	m_duration_in_s = duration;
}

void CMediaItem::setTitle(string title)
{
	m_title = title;
}

void CMediaItem::setYear(int year)
{
	m_year = year;
}
