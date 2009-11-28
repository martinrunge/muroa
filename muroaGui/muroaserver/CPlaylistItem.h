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
	CPlaylistItem();
	virtual ~CPlaylistItem();

    void setText(QString text);

private:
    void assembleText();

};

#endif /* CPLAYLISTITEM_H_ */
