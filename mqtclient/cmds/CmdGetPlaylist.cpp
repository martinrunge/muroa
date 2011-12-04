/*
 * CmdGetPlaylist.cpp
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#include "CmdGetPlaylist.h"

using namespace std;

CmdGetPlaylist::CmdGetPlaylist(unsigned knownRev, QObject * parent ) : CmdBase( GET_PLAYLIST, knownRev, parent ) {

}

CmdGetPlaylist::~CmdGetPlaylist() {

}

