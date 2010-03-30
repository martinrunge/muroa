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

#include "CItemBase.h"

class CCollectionItem :public CItemBase {
public:
	CCollectionItem(QString text);
	virtual ~CCollectionItem();

//    void setText(QString text);

//	inline QString getText() const { return m_text; };

    inline QString getAlbum() const { return m_album; }
    inline QString getArtist() const { return m_artist; }
    inline QString getFilename() const { return m_filename; }
    inline int getLengthInSec() const { return m_length_in_s; }
    inline QString getTitle() const { return m_title; }
    inline int getYear() const  { return m_year; }

    inline unsigned getHash() const { return m_hash; };

    void setAlbum(QString album);
    void setArtist(QString artist);
    void setFilename(QString filename);
    void setLengthInSec(int lengthInSec);
    void setTitle(QString title);
    void setYear(int year);

private:
    void assembleText();

//    QString m_text;

	QString m_filename;
	QString m_artist;
	QString m_album;
	QString m_title;
	int m_year;
	int m_length_in_s;

	unsigned m_hash;

	static unsigned lastUsedHash;
};

#endif /* CCOLLECTIONITEM_H_ */