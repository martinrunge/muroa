/*
 * CSimpleCmds.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef SIMPLECMDS_H_
#define SIMPLECMDS_H_

#include "Cmd.h"

namespace muroa {

class CmdPlay : public Cmd {
public:
	CmdPlay();
	virtual ~CmdPlay();
};

class CmdPause : public Cmd {
public:
	CmdPause();
	virtual ~CmdPause();
};


class CmdStop : public Cmd {
public:
	CmdStop();
	virtual ~CmdStop();
};



class CmdNext : public Cmd {
public:
	CmdNext();
	virtual ~CmdNext();
};



class CmdPrev : public Cmd {
public:
	CmdPrev();
	virtual ~CmdPrev();
};

} /* namespace muroa */
#endif /* SIMPLECMDS_H_ */
