/*
 * CMediaScanner.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CMediaScanner.h"

#include <sys/types.h>
#include <sys/socket.h>

#include "CMsgBase.h"
#include "CMsgScanDir.h"

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
	int runs = 0;

	char buffer[257];

	while(m_run) {
		ssize_t numBytes = recv(m_socket, buffer, 256, 0);
		if(numBytes == -1 && errno == EINTR) {
			// recv was interrupted by a signal
			continue;
		}
		buffer[numBytes] = '\0';
		uint32_t *u32Ptr = reinterpret_cast<uint32_t*>(buffer);

		m_dbg_file << "received message: " << hex << u32Ptr[0] << " " << u32Ptr[1] << " payloadlen:" << u32Ptr[2] << dec << endl;

		CMsgBase* msg = CMsgBase::msgFactory( buffer, numBytes );
		rc = handleMsg(msg);
		switch(rc) {
			case 1:
				m_run = false;
				break;
			case 0:
			default:
				// everything is ok
				break;
		}

//		send(m_socket, buffer, numBytes, 0);
//		runs ++;
//		if(runs == 3) {
//			m_dbg_file << "exiting ..." << endl;
//			exit(1);
//		}
	}

	m_dbg_file << "left event loop. Bye." << endl;

	return rc;
}


int CMediaScanner::handleMsg(CMsgBase* msg) {
	int rc = 0;
	int type = msg->getType();
	switch(type) {
		case E_MSG_QUIT:
			rc = 1;
			m_dbg_file << "got quit msg." << endl;
			// exit(1);
			break;

		case E_MSG_RESP:
			rc = 0;
			m_dbg_file << "other message." << endl;
			// exit(1);
			break;

		case E_MSG_SCAN_DIR:
			rc = 0;
			CMsgScanDir* scanDirMsg = reinterpret_cast<CMsgScanDir*>(msg);
			m_dbg_file << "requested to scan dir: " << scanDirMsg->getPath() << endl;
			break;

	}
	return rc;
}
