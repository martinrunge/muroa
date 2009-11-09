/*
 * CCollectionItem.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTIONITEM_H_
#define CCOLLECTIONITEM_H_

#include <QString>
#include <QStringRef>

class CCollectionItem {
public:
	CCollectionItem();
	virtual ~CCollectionItem();

	void setStringRef(QStringRef stringRef);

    inline QString getAlbum() const { return m_album; }
    inline QString getArtist() const { return m_artist; }
    inline QString getFilename() const { return m_filename; }
    inline int getLengthInSec() const { return m_length_in_s; }
    inline QString getTitle() const { return m_title; }
    inline int getYear() const  { return m_year; }
    inline void setAlbum(QString album) { m_album = album; }
    inline void setArtist(QString artist) { m_artist = artist; }
    inline void setFilename(QString filename) { m_filename = filename; }
    inline void setLengthInSec(int lengthInSec) { m_length_in_s = lengthInSec; }
    inline void setTitle(QString title) { m_title = title; }
    inline void setYear(int year) { m_year = year; }

private:
	QString m_filename;
	QString m_artist;
	QString m_album;
	QString m_title;
	int m_year;
	int m_length_in_s;
};

#endif /* CCOLLECTIONITEM_H_ */
