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

	// inline QString asString() const { return m_asString; };

	static QString getTitle(int col);
	static inline int getNumColumns() { return 1; };

	QVariant data(int column) const;
	bool setData(int column, QVariant data);

private:

};

#endif /* CPLAYLISTITEM_H_ */
