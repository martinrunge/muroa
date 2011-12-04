/*
 * CmdGetMediaCol.cpp
 *
 *  Created on: 29 Nov 2011
 *      Author: martin
 */

#include "CmdGetMediaCol.h"

using namespace std;

CmdGetMediaCol::CmdGetMediaCol(unsigned knownRev, QObject * parent ) : CmdBase( GET_MEDIA_COL, knownRev, parent ) {

}

CmdGetMediaCol::~CmdGetMediaCol() {
}

