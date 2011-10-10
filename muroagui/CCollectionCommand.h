/*
 * CCollectionCommand.h
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#ifndef CCOLLECTIONCOMMAND_H_
#define CCOLLECTIONCOMMAND_H_

#include "CCommandBase.h"

class CCollectionCommand: public CCommandBase {
public:
	CCollectionCommand( int knownRev, QObject * parent = 0 );
	CCollectionCommand( int knownRev, const QString& diff, QObject * parent = 0 );
	virtual ~CCollectionCommand();

	void setData(const QString& diff);

	void timeout();

};

#endif /* CCOLLECTIONCOMMAND_H_ */
