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

	int sock, othersock;

	if( argc < 2 ) {
		cerr << "missing arguments." << endl;
	}
	else {
		sock = strtol(argv[1], NULL, 10);
	}

	CMediaScanner mediascanner(sock);
	int rc = mediascanner.run();
	return rc;
}
