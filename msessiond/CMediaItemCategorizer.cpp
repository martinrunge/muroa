/*
 * CMediaItemCategorizer.cpp
 *
 *  Created on: 8 Jan 2012
 *      Author: martin
 */

#include "CMediaItemCategorizer.h"

#include <CRootItem.h>
#include <CMediaItem.h>

namespace muroa {

CMediaItemCategorizer::CMediaItemCategorizer() {
}

CMediaItemCategorizer::~CMediaItemCategorizer() {
}

CRootItem* CMediaItemCategorizer::categorize(std::vector<CMediaItem*> *mItems ) {
	CRootItem* ri = new CRootItem;
	CCategoryItem* parent;

	for( std::vector<CMediaItem*>::iterator it = mItems->begin(); it != mItems->end(); it++) {
		string path = "/" + (*it)->getArtist() + "/" + (*it)->getAlbum();
		parent = ri->mkPath(path);
		ri->addContentItem(*it, parent);
	}

	return ri;
}

} /* namespace muroa */
