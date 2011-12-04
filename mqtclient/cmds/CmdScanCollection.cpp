/*
 * CmdScanCollection.cpp
 *
 *  Created on: 30 Nov 2011
 *      Author: martin
 */

#include "CmdScanCollection.h"

CmdScanCollection::CmdScanCollection(unsigned knownRev, QObject * parent) : CmdBase( SCAN_COLLECTION, knownRev, parent ) {
}

CmdScanCollection::~CmdScanCollection() {
}

