/*
 * CMediaItemCategorizer.h
 *
 *  Created on: 8 Jan 2012
 *      Author: martin
 */

#ifndef CMEDIAITEMCATEGORIZER_H_
#define CMEDIAITEMCATEGORIZER_H_

#include <vector>

namespace muroa {

class CMediaItem;
class CRootItem;

class CMediaItemCategorizer {
public:
	CMediaItemCategorizer();
	virtual ~CMediaItemCategorizer();

	static CRootItem* categorize(std::vector<CMediaItem*> *mItems );
};

} /* namespace muroa */
#endif /* CMEDIAITEMCATEGORIZER_H_ */
