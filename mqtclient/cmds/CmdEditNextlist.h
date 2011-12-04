/*
 * CNextlistCommand.h
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#ifndef CMD_EDIT_NEXTLIST_H_
#define CMD_EDIT_NEXTLIST_H_

#include "CmdBase.h"

class CmdEditNextlist: public CmdBase {
public:
	CmdEditNextlist(unsigned knownRev, QObject * parent = 0 );
	CmdEditNextlist(unsigned knownRev, const std::string& diff, QObject * parent = 0);
	virtual ~CmdEditNextlist();

	void setData(const std::string& diff);

	void timeout();
};

#endif /* CMD_EDIT_NEXTLIST_H_ */
