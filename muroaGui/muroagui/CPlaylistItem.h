/*
 * CPlaylistItem.h
 *
 *  Created on: 28 Nov 2009
 *      Author: martin
 */

#ifndef CPLAYLISTITEM_H_
#define CPLAYLISTITEM_H_

#include "CItemBase.h"

class CPlaylistItem: public CItemBase {
public:
	CPlaylistItem(QString itemStr);
	virtual ~CPlaylistItem();

	inline QString asString() const { return m_key; };

	static QString getTitle(int col);
	static inline int getNumColumns() { return 1; };

private:
	QString m_key;

};

#endif /* CPLAYLISTITEM_H_ */
