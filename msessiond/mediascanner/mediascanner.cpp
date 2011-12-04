/*
 * mediascanner.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CMediaScanner.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]) {

	CMediaScanner mediascanner;
	int rc = mediascanner.run();
	return rc;
}
