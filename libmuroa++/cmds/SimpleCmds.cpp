/*
 * CSimpleCmds.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "SimpleCmds.h"

namespace muroa {

CmdPlay::CmdPlay() : Cmd(Cmd::PLAY) {
	// TODO Auto-generated constructor stub

}

CmdPlay::~CmdPlay() {
	// TODO Auto-generated destructor stub
}

CmdPause::CmdPause() : Cmd(Cmd::PAUSE) {
	// TODO Auto-generated constructor stub
}

CmdPause::~CmdPause() {
	// TODO Auto-generated destructor stub
}


CmdStop::CmdStop() : Cmd(Cmd::STOP) {
	// TODO Auto-generated constructor stub

}

CmdStop::~CmdStop() {
	// TODO Auto-generated destructor stub
}


CmdNext::CmdNext() : Cmd(Cmd::NEXT) {
	// TODO Auto-generated constructor stub

}

CmdNext::~CmdNext() {
	// TODO Auto-generated destructor stub
}


CmdPrev::CmdPrev() : Cmd(Cmd::PREV)  {
	// TODO Auto-generated constructor stub

}

CmdPrev::~CmdPrev() {
	// TODO Auto-generated destructor stub
}




} /* namespace muroa */
