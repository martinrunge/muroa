/*
 * CmdScanCollection.h
 *
 *  Created on: 30 Nov 2011
 *      Author: martin
 */

#ifndef CMDSCANCOLLECTION_H_
#define CMDSCANCOLLECTION_H_

#include "CmdBase.h"

class CmdScanCollection: public CmdBase {
public:
	CmdScanCollection(unsigned knownRev, QObject * parent = 0 );
	virtual ~CmdScanCollection();
};

#endif /* CMDSCANCOLLECTION_H_ */
