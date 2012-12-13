/*
 * CSimpleCmds.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "SimpleCmds.h"

namespace muroa {

CmdPlay::CmdPlay() : Cmd(Cmd::PLAY) {
}

CmdPlay::~CmdPlay() {
}

CmdPause::CmdPause() : Cmd(Cmd::PAUSE) {
}

CmdPause::~CmdPause() {
}


CmdStop::CmdStop() : Cmd(Cmd::STOP) {
}

CmdStop::~CmdStop() {
}


CmdNext::CmdNext() : Cmd(Cmd::NEXT) {
}

CmdNext::~CmdNext() {
}


CmdPrev::CmdPrev() : Cmd(Cmd::PREV)  {
}

CmdPrev::~CmdPrev() {
}




} /* namespace muroa */
