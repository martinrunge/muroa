/*
 * CMediaItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CMediaItem.h"

#include <sstream>
#include <functional>

using namespace std;

CMediaItem::CMediaItem() {
}

CMediaItem::~CMediaItem() {
}

void CMediaItem::setAlbum(string album)
{
	m_album = album;
	rehash();
}

void CMediaItem::setArtist(string artist)
{
	m_artist = artist;
	rehash();
}

void CMediaItem::setFilename(string filename)
{
	m_filename = filename;
	rehash();
}

void CMediaItem::setDuration(int duration)
{
	m_duration_in_s = duration;
	rehash();
}

void CMediaItem::setTitle(string title)
{
	m_title = title;
	rehash();
}

void CMediaItem::setYear(int year)
{
	m_year = year;
	rehash();
}


void CMediaItem::rehash() {
	stringstream ss;

	ss << m_filename << "," << m_artist << "," << m_album << "," << m_title << "," << m_year << "," << m_duration_in_s;

	m_hash = hash<string>()( ss.str() );
	ss << "," << m_hash;

	m_asString = ss.str();
}

bool CMediaItem::operator==(const CMediaItem& other) {
	bool rc = true;

	if( m_filename.compare( other.m_filename ) != 0 ) {
		rc = false;
	}

	if( m_artist.compare( other.m_artist ) != 0 ) {
		rc = false;
	}

	if( m_album.compare( other.m_album ) != 0 ) {
		rc = false;
	}

	if( m_title.compare( other.m_title ) != 0 ) {
		rc = false;
	}

	if( m_year != other.m_year ) {
		rc = false;
	}

	if( m_duration_in_s != other.m_duration_in_s ) {
		rc = false;
	}

	if( m_hash != other.m_hash ) {
		rc = false;
	}

	if( m_asString.compare( other.m_asString ) != 0 ) {
		rc = false;
	}

	return rc;
}


