/*
 * CMediaItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CMediaItem.h"
#include "CCategoryItem.h"
#include "CRootItem.h"
#include "CUtils.h"

#include <sstream>
#include <functional>
#include <stdexcept>
#include <algorithm>

#include <iostream>

using namespace std;


CMediaItem::CMediaItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) : IContentItem( root_item, parent, CItemType::E_MEDIAITEM ) {
	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
}

CMediaItem::CMediaItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent)
   : IContentItem( root_item, parent, CItemType::E_MEDIAITEM ) {

	m_text = text;
	// first section is handled by CItemBase
	size_t lpos, rpos;
	// lpos = m_text.find('\t', 1) + 1;
	lpos = 0;

	rpos = m_text.find('\t', lpos);
	m_filename = text.substr(lpos, rpos - lpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	m_artist = text.substr(lpos, rpos - lpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	m_album = text.substr(lpos, rpos - lpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	m_title = text.substr(lpos, rpos - lpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	string yearStr = text.substr(lpos, rpos - lpos);
	m_year = CUtils::str2long(yearStr.c_str());
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	string durationStr = text.substr(lpos, rpos - lpos);
	m_duration_in_s = CUtils::str2long(durationStr.c_str());
	lpos = rpos + 1;

	string hashStr = text.substr(lpos);
	m_hash = CUtils::str2uint32(hashStr.c_str());

	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
	rehash();
}

CMediaItem::~CMediaItem() {
	m_root_item->delContentPtr(CItemType(CItemType::E_MEDIAITEM), m_hash );
}

void CMediaItem::setAlbum(string album)
{
	replaceTabs(album);
	m_album = album;
	rehash();
}

void CMediaItem::setArtist(string artist)
{
	replaceTabs(artist);
	m_artist = artist;
	rehash();
}

void CMediaItem::setFilename(string filename)
{
	replaceTabs(filename);
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
	replaceTabs(title);
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

	if( m_parent ) {
		ss << m_parent->getPath();
	}
	ss << "\tM\t" << m_filename << "\t" << m_artist << "\t" << m_album << "\t" << m_title << "\t" << m_year << "\t" << m_duration_in_s;
	//ss << "m\t" <<  m_filename << "\t" << m_artist << "\t" << m_album << "\t" << m_title << "\t" << m_year << "\t" << m_duration_in_s;

	uint32_t oldhash = m_hash;
	m_hash = hash( ss.str() );
	ss << "\t" << m_hash << endl;

	if( m_hash != oldhash ) {
		m_root_item->setContentPtr(CItemType(CItemType::E_MEDIAITEM), this, m_hash );
 		m_root_item->delContentPtr(CItemType(CItemType::E_MEDIAITEM), oldhash );  // would delete this
	}

	m_text = ss.str();
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

	if( m_text.compare( other.m_text ) != 0 ) {
		rc = false;
	}

	return rc;
}


string CMediaItem::serialize(bool asDiff) {
	if(asDiff) {
		string diffline("+");
		diffline.append(m_text);
		return diffline;
	}
	else {
		return m_text;
	}
}
