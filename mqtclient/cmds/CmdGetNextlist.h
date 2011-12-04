/*
 * CmdGetNextlist.h
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#ifndef CMDGETNEXTLIST_H_
#define CMDGETNEXTLIST_H_

#include "CmdBase.h"

class CmdGetNextlist: public CmdBase {
public:
	CmdGetNextlist(unsigned knownRev, QObject * parent = 0 );
	virtual ~CmdGetNextlist();
};

#endif /* CMDGETNEXTLIST_H_ */
