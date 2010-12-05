/*
 * CMediaItem.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CMEDIAITEM_H_
#define CMEDIAITEM_H_

#include <string>

class CMediaItem {
public:
	CMediaItem();
	virtual ~CMediaItem();

    inline std::string getAlbum() const { return m_album; }
    inline std::string getArtist() const { return m_artist; }
    inline std::string getFilename() const { return m_filename; }
    inline int getDuration() const { return m_duration_in_s; }
    inline std::string getTitle() const { return m_title; }
    inline int getYear() const  { return m_year; }

    void setAlbum(std::string album);

    void setArtist(std::string artist);
    void setFilename(std::string filename);
    void setDuration(int duration);
    void setTitle(std::string title);
    void setYear(int year);

	inline int getHash() { return m_hash; };

private:

    std::string m_filename;
    std::string m_artist;
    std::string m_album;
    std::string m_title;
	int m_year;
	int m_duration_in_s;

	unsigned m_hash;

    std::string m_asString;
	void rehash();
};

#endif /* CCOLLECTIONITEM_H_ */
