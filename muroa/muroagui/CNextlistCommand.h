/*
 * CNextlistCommand.h
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#ifndef CNEXTLISTCOMMAND_H_
#define CNEXTLISTCOMMAND_H_

#include "CCommandBase.h"

class CNextlistCommand: public CCommandBase {
public:
	CNextlistCommand(int knownRev, QObject * parent = 0 );
	CNextlistCommand(int knownRev, const QString& diff, QObject * parent = 0);
	virtual ~CNextlistCommand();

	void setData(const QString& diff);

	void timeout();
};

#endif /* CNEXTLISTCOMMAND_H_ */
