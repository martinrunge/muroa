/*
 * CStateDbUpdater.h
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#ifndef CSTATEDBUPDATER_H_
#define CSTATEDBUPDATER_H_

#include "../CStateDbBase.h"

#include <string>
#include <vector>

class CMediaItem;

class CStateDbUpdater : public CStateDbBase {
public:
	CStateDbUpdater(std::string dbFileName);
	virtual ~CStateDbUpdater();

	int open();
	int close();


	void appendCollectionRev(std::vector<CMediaItem*> *collection);
	std::vector<CMediaItem*> *getCollectionRev(int rev);
};

#endif /* CSTATEDBUPDATER_H_ */
