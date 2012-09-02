/*
 * CmdEditSessionState.h
 *
 *  Created on: 01 Sep 2012
 *      Author: martin
 */

#ifndef CMD_EDIT_SESSION_STATE_H_
#define CMD_EDIT_SESSION_STATE_H_

#include "CmdBase.h"

class CmdEditSessionState: public CmdBase {
public:
	CmdEditSessionState(unsigned knownRev, QObject * parent = 0 );
	CmdEditSessionState(unsigned knownRev, const std::string& diff, QObject * parent = 0);
	virtual ~CmdEditSessionState();

	void setData(const std::string& diff);

	void timeout();
};

#endif /* CMD_EDIT_SESSION_STATE_H_ */
