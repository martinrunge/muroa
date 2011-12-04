/*
 * CmdGetMediaCol.h
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#ifndef CMDGETMEDIACOL_H_
#define CMDGETMEDIACOL_H_

#include "CmdBase.h"

class CmdGetMediaCol: public CmdBase {
public:
	CmdGetMediaCol(unsigned knownRev, QObject * parent = 0 );
	virtual ~CmdGetMediaCol();


};

#endif /* CMDGETMEDIACOL_H_ */
