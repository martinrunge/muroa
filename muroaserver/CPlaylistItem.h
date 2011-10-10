/*
 * CPlaylistItem.h
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#ifndef CPLAYLISTITEM_H_
#define CPLAYLISTITEM_H_

#include "CItemBase.h"

class CPlaylistItem : public CItemBase{
public:
	CPlaylistItem(QString text);
	CPlaylistItem(unsigned  hash);
	CPlaylistItem(const CPlaylistItem& other);
	virtual ~CPlaylistItem();

	unsigned getCollectionHash() const { return m_collectionHash; };

//    void setText(QString text);
	inline void rehash() {  };

	CPlaylistItem& operator=(const CPlaylistItem& rhs);

private:
    void assembleText();

    unsigned m_collectionHash;

};

#endif /* CPLAYLISTITEM_H_ */
