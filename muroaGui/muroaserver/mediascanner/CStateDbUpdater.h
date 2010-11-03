/*
 * CStateDbUpdater.h
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#ifndef CSTATEDBUPDATER_H_
#define CSTATEDBUPDATER_H_

#include "CStateDbBase.h"

#include <string>

class CStateDbUpdater : public CStateDbBase {
public:
	CStateDbUpdater(std::string dbFileName);
	virtual ~CStateDbUpdater();
};

#endif /* CSTATEDBUPDATER_H_ */
