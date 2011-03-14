/*
 * CPlaylistCommand.h
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#ifndef CPLAYLISTCOMMAND_H_
#define CPLAYLISTCOMMAND_H_

#include "CCommandBase.h"

class CPlaylistCommand: public CCommandBase {
public:
	CPlaylistCommand(int knownRev, QObject * parent = 0 );
	CPlaylistCommand(int knownRev, const QString& diff, QObject * parent = 0 );
	virtual ~CPlaylistCommand();

	void setData(const QString& diff);

	void timeout();
};

#endif /* CPLAYLISTCOMMAND_H_ */
