/*
 * CMediaScanner.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CMediaScanner.h"

#include "CFsScanner.h"
#include "CStateDbUpdater.h"
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "CMsgBase.h"
#include "CMsgOpenDb.h"
#include "CMsgScanDir.h"
#include "CMsgProgress.h"
#include "CMsgFinished.h"

using namespace std;

CMediaScanner::CMediaScanner(int sock_fd) : CEventLoop(sock_fd), m_fs_scanner(0), m_stateDbUpdater(0)
{
	  m_dbg_file.open("mediascanner.log");
	  m_dbg_file << "CMediaScanner::CMediaScanner(" << sock_fd << ")" << endl;

	  m_fs_scanner = new CFsScanner(this);

	  vector<string> types;
	  types.push_back(".mp3");
	  //types.push_back(".ogg");
	  m_fs_scanner->setFileTypes(types);
}

CMediaScanner::~CMediaScanner() {
	if(m_stateDbUpdater != 0) {
		delete m_stateDbUpdater;
		m_stateDbUpdater = 0;
	}
	delete m_fs_scanner;
	m_dbg_file.close();
}


int CMediaScanner::handleMsg(CMsgBase* msg) {
	int rc = 0;
	int type = msg->getType();
	switch(type) {
		case E_MSG_QUIT:
			rc = 1;
			m_dbg_file << "got quit msg." << endl;
			if(m_stateDbUpdater != 0) {
				m_stateDbUpdater->close();
			}
			// exit(1);
			break;

		case E_MSG_RESP:
			rc = 0;
			m_dbg_file << "other message." << endl;
			// exit(1);
			break;

		case E_MSG_OPEN_DB:
		{
			rc = 0;
			CMsgOpenDb* openDbMsg = reinterpret_cast<CMsgOpenDb*>(msg);
			m_dbg_file << "open db: " << openDbMsg->getDbPath() << endl;
            m_stateDbUpdater = new CStateDbUpdater( openDbMsg->getDbPath() );
            m_stateDbUpdater->open();
			break;
		}
		case E_MSG_SCAN_DIR:
		{
			rc = 0;
			CMsgScanDir* scanDirMsg = reinterpret_cast<CMsgScanDir*>(msg);
			m_fs_scanner->scanDir( scanDirMsg->getPath() );
			m_dbg_file << "requested to scan dir: " << scanDirMsg->getPath() << endl;

			// delete scanDirMsg;
			break;
		}
		case E_MSG_FINISHED:
		{
			CMsgFinished* finishedMsg = reinterpret_cast<CMsgFinished*>(msg);
			m_dbg_file << "A job finished." << endl;
			std::vector<CMediaItem*>* collection = m_fs_scanner->finishScan();
			m_stateDbUpdater->appendCollectionRev(collection);
			break;
		}
		case E_MSG_PROGRESS:
		{
			CMsgProgress* progressMsg = reinterpret_cast<CMsgProgress*>(msg);
			m_dbg_file << "Progress report:" << progressMsg->getProgress() << endl;
			break;
		}
	}
	delete msg;
	return rc;
}


