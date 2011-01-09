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
#include "CMsgResponse.h"
#include "CMsgCollectionChanged.h"

using namespace std;

CMediaScanner::CMediaScanner(int sock_fd) : CEventLoop(sock_fd), m_fs_scanner(0), m_stateDbUpdater(0), m_progress(-1)
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


bool CMediaScanner::handleMsg(CMsgBase* msg) {
	m_dbg_file << "CMediaScanner::handleMsg" << endl;
	bool rc = true;
	uint32_t type = msg->getType();
	try {
		switch(type) {
			case E_MSG_QUIT:
				rc = false;
				m_dbg_file << "got quit msg." << endl;
				if(m_stateDbUpdater != 0) {
					m_stateDbUpdater->close();
				}
				// exit(1);
				break;

			case E_MSG_RESP:
				m_dbg_file << "other message." << endl;
				// exit(1);
				break;

			case E_MSG_OPEN_DB:
			{
				CMsgOpenDb* openDbMsg = reinterpret_cast<CMsgOpenDb*>(msg);
				std::string path = openDbMsg->getDbPath();
				m_dbg_file << "open db: " << path << endl;
				m_stateDbUpdater = new CStateDbUpdater( openDbMsg->getDbPath() );
				m_stateDbUpdater->open();
				break;
			}
			case E_MSG_SCAN_DIR:
			{
				if(m_stateDbUpdater == 0) {
					// no MsgOpenDB received yet -> scan not possible
					throw CApiMisuseException("State DB not opened yet.");
				}
				CMsgScanDir* scanDirMsg = reinterpret_cast<CMsgScanDir*>(msg);
				m_fs_scanner->scanDir( scanDirMsg->getPath(), scanDirMsg->getID() );
				m_progress = 0;
				m_dbg_file << "requested to scan dir: " << scanDirMsg->getPath() << endl;

				// delete scanDirMsg;
				break;
			}
			case E_MSG_FINISHED:
			{
				CMsgFinished* finishedMsg = reinterpret_cast<CMsgFinished*>(msg);
				uint32_t jobID = finishedMsg->getJobID();
				m_dbg_file << "A job with ID " << jobID << " finished." << endl;
				if( jobID == m_fs_scanner->getJobID()) {     // make sure scandir job finished

					std::vector<CMediaItem*>* collection = m_fs_scanner->finishScan();
					m_stateDbUpdater->appendCollectionRev(collection);
					CMsgFinished *finiNotification = new CMsgFinished(jobID);
					sendEvent(finiNotification);
					m_dbg_file << "sent finished notification to parent: [jobID " <<  finiNotification->getJobID() << "]" << endl;

					if( /*collection changed */ true  ) {
						int minRev = m_stateDbUpdater->getIntValue("CollectionRevMin");
						int maxRev = m_stateDbUpdater->getIntValue("CollectionRevMax");
						CMsgCollectionChanged* colChanged = new CMsgCollectionChanged( maxRev, minRev, maxRev );
						sendEvent(colChanged);
						m_dbg_file << "sent collectionChanged notification to parent." << endl;

					}
				}
				break;
			}
			case E_MSG_PROGRESS:
			{
				CMsgProgress* progressMsg = reinterpret_cast<CMsgProgress*>(msg);
				m_dbg_file << "Progress report:" << progressMsg->getProgress() << endl;
				m_progress = progressMsg->getProgress();
				CMsgProgress* progNotification = new CMsgProgress( progressMsg->getJobID(), progressMsg->getProgress());
				sendEvent(progNotification);
				m_dbg_file << "sent progress notification to parent: [jobID " <<  progressMsg->getJobID() << ": " << progressMsg->getProgress() << "] " << endl;
				break;
			}
		}
	}
	catch(CApiMisuseException apiEx) {
		CMsgResponse* response = new CMsgResponse(msg->getID(), -1, apiEx.what() );
		sendEvent(response);
	}

	delete msg;
	return rc;
}


