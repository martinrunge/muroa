/*
 * CMediaItem.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CMEDIAITEM_H_
#define CMEDIAITEM_H_

#include "CItemBase.h"

#include <string>

class CMediaItem : public CItemBase {
public:
	CMediaItem(CCategoryItem*  parent, int posInParent = -1);
	CMediaItem(std::string text, CCategoryItem*  parent, int posInParent = -1);
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

	inline uint32_t getHash() { return m_hash; };

	bool operator==(const CMediaItem& other);
	inline bool operator!=(const CMediaItem& other){ return !operator==(other); };

	void addChild(CMediaItem* newMediaItem, int pos = -1);
	int numChildren() { return 0; };
	CItemBase* childAt(unsigned row) { return 0; };
	virtual unsigned childPos(CItemBase* child) { return 0; };

	std::string serialize(bool asDiff = false);

	static uint32_t hash( std::string stdstr );

private:

    std::string m_filename;
    std::string m_artist;
    std::string m_album;
    std::string m_title;
	int m_year;
	int m_duration_in_s;

	uint32_t m_hash;

	void rehash();
};

#endif /* CCOLLECTIONITEM_H_ */
