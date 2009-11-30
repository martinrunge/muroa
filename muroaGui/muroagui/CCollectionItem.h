/*
 * CCollectionItem.h
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTIONITEM_H_
#define CCOLLECTIONITEM_H_

#include "CItemBase.h"

#include <QString>

class CCollectionItem : public CItemBase {
public:
	CCollectionItem(QString itemStr);
	virtual ~CCollectionItem();

    inline QString getAlbum() const { return m_album; }
    inline QString getArtist() const { return m_artist; }
    inline QString getFilename() const { return m_filename; }
    inline int getLengthInSec() const { return m_length_in_s; }
    inline QString getTitle() const { return m_title; }
    inline int getYear() const  { return m_year; }

    inline unsigned long getHash() { return m_hash; };

    inline void setAlbum(QString album) { m_album = album; }
    inline void setArtist(QString artist) { m_artist = artist; }
    inline void setFilename(QString filename) { m_filename = filename; }
    inline void setLengthInSec(int lengthInSec) { m_length_in_s = lengthInSec; }
    inline void setTitle(QString title) { m_title = title; }
    inline void setYear(int year) { m_year = year; }

	static QString getTitle(int col);
	static inline int getNumColumns() { return 5; };

private:
	QString m_filename;
	QString m_artist;
	QString m_album;
	QString m_title;
	int m_year;
	int m_length_in_s;

	unsigned long m_hash;
};

#endif /* CCOLLECTIONITEM_H_ */
