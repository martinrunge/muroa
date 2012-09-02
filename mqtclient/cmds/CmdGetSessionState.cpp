/*
 * CmdGetSessionState.cpp
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#include "CmdGetSessionState.h"

using namespace std;

CmdGetSessionState::CmdGetSessionState(unsigned knownRev, QObject * parent ) : CmdBase( GET_SESSION_STATE, knownRev, parent ) {

}

CmdGetSessionState::~CmdGetSessionState() {

}

