/*
 * CStateDbUpdater.h
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#ifndef CSTATEDBUPDATER_H_
#define CSTATEDBUPDATER_H_

#include <CStateDbBase.h>

#include <string>
#include <vector>

namespace muroa {
  class CMediaItem;
}

class CStateDbUpdater : public CStateDbBase {
public:
	CStateDbUpdater(std::string dbFileName);
	virtual ~CStateDbUpdater();

	int open();
	int close();


	int appendCollectionRev(std::vector<muroa::CMediaItem*> *collection);
	std::vector<muroa::CMediaItem*> *getCollectionRev(int rev);
};

#endif /* CSTATEDBUPDATER_H_ */
