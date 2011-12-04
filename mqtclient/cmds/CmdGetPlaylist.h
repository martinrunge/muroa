/*
 * CmdGetPlaylist.h
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#ifndef CMDGETPLAYLIST_H_
#define CMDGETPLAYLIST_H_

#include "CmdBase.h"

class CmdGetPlaylist: public CmdBase {
public:
	CmdGetPlaylist(unsigned knownRev, QObject * parent = 0 );
	virtual ~CmdGetPlaylist();
};

#endif /* CMDGETPLAYLIST_H_ */
