/*
 * CMediaScanner.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CMediaScanner.h"

#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "CMsgBase.h"
#include "CMsgScanDir.h"
#include "CMsgProgress.h"
#include "CMsgFinished.h"

using namespace std;

CMediaScanner::CMediaScanner(int sock_fd) : CEventLoop(sock_fd)
{
	  m_dbg_file.open("mediascanner.log");
	  m_dbg_file << "CMediaScanner::CMediaScanner(" << sock_fd << ")" << endl;

}

CMediaScanner::~CMediaScanner() {
	m_dbg_file.close();
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
		{
			rc = 0;
			CMsgScanDir* scanDirMsg = reinterpret_cast<CMsgScanDir*>(msg);
			m_dbg_file << "requested to scan dir: " << scanDirMsg->getPath() << endl;
			break;
		}
		case E_MSG_FINISHED:
		{
			CMsgFinished* finishedMsg = reinterpret_cast<CMsgFinished*>(msg);
			m_dbg_file << "A job finished." << endl;
			break;
		}
		case E_MSG_PROGRESS:
		{
			CMsgProgress* progressMsg = reinterpret_cast<CMsgProgress*>(msg);
			m_dbg_file << "Progress report:" << progressMsg->getProgress() << endl;
			break;
		}
	}
	return rc;
}


