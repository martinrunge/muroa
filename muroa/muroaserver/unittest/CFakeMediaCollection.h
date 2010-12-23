/*
 * CFakeMediaCollection.h
 *
 *  Created on: 21 Dec 2010
 *      Author: martin
 */

#ifndef CFAKEMEDIACOLLECTION_H_
#define CFAKEMEDIACOLLECTION_H_

#include <string>
#include <vector>

class CMediaItem;

class CFakeMediaCollection {
public:
	CFakeMediaCollection(std::string indir);
	virtual ~CFakeMediaCollection();

	std::vector<CMediaItem*>* collectionWithoutFiles( int size );

private:
	std::string m_rootdir;
};

#endif /* CFAKEMEDIACOLLECTION_H_ */
