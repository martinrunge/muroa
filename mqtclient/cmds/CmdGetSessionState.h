/*
 * CmdGetSessionState.h
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#ifndef CMDGETSESSIONSTATE_H_
#define CMDGETSESSIONSTATE_H_

#include "CmdBase.h"

class CmdGetSessionState: public CmdBase {
public:
	CmdGetSessionState(unsigned knownRev, QObject * parent = 0 );
	virtual ~CmdGetSessionState();
};

#endif /* CMDGETSESSIONSTATE_H_ */
