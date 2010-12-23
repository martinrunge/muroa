/*
 * CFakeMediaCollection.cpp
 *
 *  Created on: 21 Dec 2010
 *      Author: martin
 */

#include "CFakeMediaCollection.h"

#include "../mediascanner/CMediaItem.h"
#include <sstream>
using namespace std;

CFakeMediaCollection::CFakeMediaCollection(string indir) : m_rootdir(indir) {
	// TODO Auto-generated constructor stub

}

CFakeMediaCollection::~CFakeMediaCollection() {
	// TODO Auto-generated destructor stub
}


std::vector<CMediaItem*>* CFakeMediaCollection::collectionWithoutFiles( int size ) {
	vector<CMediaItem*>* collection = new vector<CMediaItem*>;

	for(int i = 0; i < size; i++ ) {
		CMediaItem* item = new CMediaItem();

		ostringstream ss;
		ss << "Test Album " << i;
		item->setAlbum( ss.str() );

		ss.str("");
		ss << "Test Artist " << i;
		item->setArtist(ss.str());

		ss.str("");
		ss << "Test Title " << i;
		item->setTitle(ss.str());

		item->setYear( 1990 + i % 20 );
		item->setDuration( 240 );

		collection->push_back(item);
	}

	return collection;
}
