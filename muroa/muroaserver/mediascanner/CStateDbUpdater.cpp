/*
 * CStateDbUpdater.cpp
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#include "CStateDbUpdater.h"
#include "CMediaItem.h"

#include <assert.h>
using namespace std;

CStateDbUpdater::CStateDbUpdater(std::string dbFileName) : CStateDbBase(dbFileName) {

}

CStateDbUpdater::~CStateDbUpdater() {

}

int CStateDbUpdater::open() {
	int rc = CStateDbBase::open();
	if( rc == 0 ){

	}
}

int CStateDbUpdater::close() {
	int rc = CStateDbBase::close();
	return rc;
}

void CStateDbUpdater::appendCollectionRev(std::vector<CMediaItem*> *collection) {
	beginTansaction();

	string colMaxRev = getValue("CollectionRevMax");
	errno = 0;
	int maxRev = strtol(colMaxRev.c_str(), NULL, 10);
	assert (errno  == 0);

	for(int i = 0; i < collection->size(); i++) {
		CMediaItem* item = collection->at(i);
		updateMediaItem(item);
		updateCollectionRevItem(i, item->getHash(), maxRev );
	}

	setValue("CollectionRevMax", maxRev + 1);

	endTransaction();
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
