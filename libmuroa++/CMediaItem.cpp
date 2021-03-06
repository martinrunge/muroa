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
#include <cassert>

using namespace std;
namespace muroa {

CMediaItem::CMediaItem(const CMediaItem& other, CRootItem* const root_item, CCategoryItem* const parent) :
            IContentItem( root_item, parent, CItemType::E_MEDIAITEM )
{
    m_filename = other.m_filename;
    m_artist = other.m_artist;
    m_album = other.m_album;
    m_title = other.m_title;
    m_year = other.m_year;
    m_duration_in_s = other.m_duration_in_s;
    m_hash = other.m_hash;
    m_text = other.m_text;
}


CMediaItem::CMediaItem(CRootItem *root_item, CCategoryItem*  parent, int posInParent) : IContentItem( root_item, parent, CItemType::E_MEDIAITEM ) {
	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
}

CMediaItem::CMediaItem(CRootItem *root_item, std::string text, CCategoryItem*  parent, int posInParent) throw(ExMalformedPatch)
   : IContentItem( root_item, parent, CItemType::E_MEDIAITEM )   {

	size_t lpos, rpos;
	lpos = 0;
	int num_tabs = 0;
	int error_in_section = 0;
	bool serialisationNeeded = true;

	try {
		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string typeStr = text.substr(lpos, rpos - lpos);
			if(typeStr.compare(CItemType::getString(m_item_type)) == 0 ) {
				// serialisation started with type string 'M' -> input string is suitable for m_text
				lpos = rpos + 1;
				m_text = text;
				serialisationNeeded = false;
				num_tabs++;
			}
		}
		else {
			throw ExMalformedPatch("error parsing first field (expecting type string or filename), terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_filename = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
			num_tabs++;
		}
		else {
			throw ExMalformedPatch("error parsing filename field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_artist = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
			num_tabs++;
		}
		else {
			throw ExMalformedPatch("error parsing artist field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_album = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
			num_tabs++;
		}
		else {
			throw ExMalformedPatch("error parsing album field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			m_title = text.substr(lpos, rpos - lpos);
			lpos = rpos + 1;
			num_tabs++;
		}
		else {
			throw ExMalformedPatch("error parsing title field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string yearStr = text.substr(lpos, rpos - lpos);
			m_year = CUtils::str2long(yearStr);
			lpos = rpos + 1;
			num_tabs++;
		}
		else {
			throw ExMalformedPatch("error parsing year field, terminating tab char is missing.", -1);
		}

		rpos = text.find('\t', lpos);
		if( rpos != string::npos ) {
			string durationStr = text.substr(lpos, rpos - lpos);
			m_duration_in_s = CUtils::str2long(durationStr);
			lpos = rpos + 1;
			num_tabs++;
		}
		else {
			throw ExMalformedPatch("error parsing duration field, terminating tab char is missing.", -1);
		}

		string hashStr;
		rpos = text.find_first_of("\t\n", lpos);
		if(rpos != string::npos ) {
			hashStr = text.substr(lpos, rpos - lpos);  // there may be extra data after this field as long as it is separated by a tab char.
		}
		else {
			hashStr = text.substr(lpos);
		}
		m_hash = CUtils::str2uint32(hashStr);

		if(m_parent) {
			m_parent->addChild(this, posInParent);
		}

		if(serialisationNeeded) {
			assembleText();
			// from here on "m_text" is surely valid
		}

		if( m_root_item != 0) {
			m_root_item->setContentPtr(CItemType(CItemType::E_MEDIAITEM), this, m_hash );
		}
		if(m_text.rfind('\n') == string::npos ) {
			m_text.append("\n");
		}
	}
	catch(std::invalid_argument& ex)
	{
		throw ExMalformedPatch(ex.what(), -1);
	}
}

CMediaItem::CMediaItem(CRootItem *root_item ) : IContentItem( root_item, 0, CItemType::E_MEDIAITEM ) {
}

//CMediaItem* CMediaItem::clone(const IContentItem& other, CRootItem *root_item, CCategoryItem*  parent) {
//    assert(other.type() == CItemType::E_MEDIAITEM);
//
//    CMediaItem* newItem = new CMediaItem(root_item, parent);
//
//    newItem->m_filename = m_filename;
//    newItem->m_artist = m_artist;
//    newItem->m_album = m_album;
//    newItem->m_title = m_title;
//    newItem->m_year = m_year;
//    newItem->m_duration_in_s = m_duration_in_s;
//    newItem->m_hash = m_hash;
//    newItem->m_text = m_text;
//
//    return newItem;
//}


void CMediaItem::setParent(CRootItem *root_item, CCategoryItem*  parent, int posInParent) {
	m_root_item = root_item;
	m_parent = parent;

	// rehash();

	if(m_parent) {
		m_parent->addChild(this, posInParent);
	}
}

CMediaItem::~CMediaItem() {
	if(m_root_item != 0) {
		m_root_item->delContentPtr(CItemType(CItemType::E_MEDIAITEM), m_hash );
	}
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

void CMediaItem::assembleText() {
	stringstream ss;
	ss << "M\t" << m_filename << "\t" << m_artist << "\t" << m_album << "\t" << m_title << "\t" << m_year << "\t" << m_duration_in_s << "\t" << m_hash << endl;
	m_text = ss.str();
}

void CMediaItem::rehash() {
	stringstream ss;
	ss << "M\t" << m_filename << "\t" << m_artist << "\t" << m_album << "\t" << m_title << "\t" << m_year << "\t" << m_duration_in_s;

	uint32_t oldhash = m_hash;
	m_hash = hash( ss.str() );
	ss << "\t" << m_hash << endl;

	if( m_hash != oldhash && m_root_item != 0) {
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
} // namespace muroa
