/*
 * CMediaScanner.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CMediaScanner.h"

#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

CMediaScanner::CMediaScanner(int sock_fd) : m_socket(sock_fd) {
	  m_dbg_file.open("mediascanner.log");
	  m_dbg_file << "CMediaScanner::CMediaScanner(" << m_socket << ")" << endl;

}

CMediaScanner::~CMediaScanner() {
	m_dbg_file.close();
}


int CMediaScanner::run() {
	int rc = 0;
	m_run=true;

	char buffer[257];

	while(m_run) {
		ssize_t numBytes = recv(m_socket, buffer, 256, 0);
		buffer[numBytes] = '\0';
		m_dbg_file << buffer << endl;
	}
	return rc;
}
