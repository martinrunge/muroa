/*
 * CStateDB.h
 *
 *  Created on: 3 Nov 2010
 *      Author: martin
 */

#ifndef CSTATEDB_H_
#define CSTATEDB_H_

#include "CStateDbBase.h"

class CStateDB: public CStateDbBase {
public:
	CStateDB(std::string dbFileName);
	virtual ~CStateDB();
};

#endif /* CSTATEDB_H_ */
