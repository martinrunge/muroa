/*
 * CStateDbUpdater.cpp
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#include "CStateDbUpdater.h"

#include <assert.h>
using namespace std;
using namespace muroa;

#include "CMediaItem.h"

CStateDbUpdater::CStateDbUpdater(std::string dbFileName) : CStateDbBase(dbFileName) {

}

CStateDbUpdater::~CStateDbUpdater() {

}

int CStateDbUpdater::open() {
	int rc = CStateDbBase::open();
	return rc;
}

int CStateDbUpdater::close() {
	int rc = CStateDbBase::close();
	return rc;
}

int CStateDbUpdater::appendCollectionRev(std::vector<CMediaItem*> *collection) {
	int nrChanges = 0;
	bool found;

	beginTansaction();

	int maxRev = getIntValue("MaxMediaColRev", found);
	assert(found == true);
	maxRev++;

	for(int i = 0; i < collection->size(); i++) {
		CMediaItem* item = collection->at(i);
		nrChanges += updateMediaItem(item);
		updateCollectionRevItem(i, item->getHash(), maxRev );
	}

	if(nrChanges > 0) {
		setValue("MaxMediaColRev", maxRev);
	}

	endTransaction();

	return nrChanges;
}


std::vector<CMediaItem*> *CStateDbUpdater::getCollectionRev(int rev) {
	beginTansaction();
	std::vector<CMediaItem*> *collection = new vector<CMediaItem*>;

	CMediaItem* item;
	int pos = 0;
	do {
		item = getMediaItemByPos(pos, rev);
		pos++;
		if(item) {
			collection->push_back(item);
		}
	} while(item != 0);

	endTransaction();
	return collection;
}
