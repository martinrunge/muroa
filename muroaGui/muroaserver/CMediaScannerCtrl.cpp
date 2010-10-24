/*
 * CMediaScannerCtrl.cpp
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#include "CMediaScannerCtrl.h"

#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

CMediaScannerCtrl::CMediaScannerCtrl() {
	// TODO Auto-generated constructor stub
}

CMediaScannerCtrl::~CMediaScannerCtrl() {
	// TODO Auto-generated destructor stub
}


void CMediaScannerCtrl::start() {
	vector<string> args;

	int sv[2];
	int retval = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
	if(retval != 0) {
		perror("CMediaScannerCtrl::start:");
	}

	m_socket = sv[0];

	stringstream ss;
	ss << sv[1];
	args.push_back(ss.str());

	m_subProcess.start("./build/mediascanner/mediascanner" , args ,0 ,0);

	send(m_socket, "hallo", 5, 0);
}
