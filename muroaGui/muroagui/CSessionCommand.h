/*
 * CSessionCommand.h
 *
 *  Created on: 12 Jun 2010
 *      Author: martin
 */

#ifndef CSESSIONCOMMAND_H_
#define CSESSIONCOMMAND_H_

#include "CCommandBase.h"

class CSessionCommand: public CCommandBase {
public:
	CSessionCommand( QString name = QString(), QObject * parent = 0 );
	virtual ~CSessionCommand();

	void timeout();

};

#endif /* CSESSIONCOMMAND_H_ */
