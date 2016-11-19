/*
 * CMediaScannerCtrl.cpp
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#include "CMediaScannerCtrl.h"
#include "CApp.h"
#include "CSession.h"
#include "sessionEx.h"


#include <signal.h>
#include <stdint.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>

#include "../mmscanner/CMsgError.h"

#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace boost::asio;

namespace muroa {

CMediaScannerCtrl::CMediaScannerCtrl(muroa::CSession *parent, io_service& io_service) : CSubProcess(io_service),
		                                                                         m_socket(io_service),
		                                                                         m_parent(parent) {
	m_bufCapacity = 20 * 256 + 1;
	m_bufPtr = new char[m_bufCapacity];
	m_bufSize = 0;


}

CMediaScannerCtrl::~CMediaScannerCtrl() {
	delete [] m_bufPtr;
}


void CMediaScannerCtrl::start() {

	vector<string> args;

	string mmscannerbin = CApp::settings().getConfigVal("msessiond.mmscanner_binary", "mmscanner/mmscanner");

	pid_t pid = CSubProcess::start(mmscannerbin , args ,0 ,0);

	if(pid == -5) {
		// already running, this is no error.
		return;
	}

	if(pid < 0) {
		ostringstream oss;
		oss << "Could not start mediascanner (tried '" << mmscannerbin << "')" << endl;
		cerr << oss.str();
		LOG4CPLUS_ERROR(CApp::logger(), oss.str());
	}
}

void CMediaScannerCtrl::stop() {
	CMsgQuit quitmsg;

	sendMsg(&quitmsg);
	terminate();
}

void CMediaScannerCtrl::terminate() {
	CMsgQuit quitmsg;
	sendMsg(&quitmsg);

	int tries = 0;
	while(isRunning()) {
		// cv_status cv_return_cause = m_cond_var.wait_for(lk, std::chrono::milliseconds(100));

		cerr << "m_cond_var.wait_for returned due to ";
//		if(cv_return_cause == cv_status::timeout) {
//			switch (tries) {
//				case 0:
//				case 1:
//					cerr << "timeout -> send SIGTERM to mediascanner process" << endl;
//					kill(m_pid, SIGTERM);
//					break;
//				case 2:
//				default:
//					cerr << "timeout -> send SIGKILL to mediascanner process" << endl;
//					kill(m_pid, SIGKILL);
//			}
//		}
//		else {
//			cerr << "notification from waitpid -> mediascanner process exited." << endl;
//		}
		tries++;
	}

	// interrupt blocking recv in loop() out
//	pthread_kill(m_thread->native_handle(), SIGUSR1);
//	if(m_thread != 0) {
//		m_thread->join();
//		delete m_thread;
//		m_thread = 0;
//	}

	CSubProcess::sendSignal(9);
}

void CMediaScannerCtrl::fromStdout(const char* buffer, int length) {
	CMsgBase* msg;
	string errormsg(buffer, length);
	cerr << "child stdout: " << errormsg;

	char* bufFreePtr = m_bufPtr + m_bufSize;
	assert( m_bufCapacity >= m_bufSize + length);
	memcpy(bufFreePtr, buffer, length);

	m_bufSize += length;

	while(m_bufSize > 0) {
		try {
			char* buf = m_bufPtr;
			int size = m_bufSize;

			cerr << "calling msgFactory: bufPtr=" << hex << (void*)buf << dec << " bufferSize=" << size << endl;
			msg = CMsgBase::msgFactory( buf, size );
			cerr << "after return: bufPtr=" << hex << (void*)buf << dec << " bufferSize=" << size << endl;

			int freeSpaceAtBeginning = buf - m_bufPtr;
			if( size < freeSpaceAtBeginning) {
				memcpy(m_bufPtr, buf, size);
			} else {
				memmove(m_bufPtr, buf, size);
			}
			m_bufSize = size;

		}
		catch(NeedMoreDataException dataEx) {
			cerr << dataEx.reason() << endl;

			// not enough data received yet to deserialize a message. So dont handle it either.
			return;
		}

		// handle the CMsg object, no matter if it was receiver via socket or eventQueue.
		bool run = handleMsg(msg);
		if(run == false) stop();
	}
}

void CMediaScannerCtrl::fromStderr(const char* buffer, int length) {
	string errormsg(buffer, length);
	cerr << "childs stderr: " << errormsg;
	LOG4CPLUS_ERROR(CApp::logger(), "mmscanner's stderr: " << errormsg << endl );
}


void CMediaScannerCtrl::sendMsg(CMsgBase *msg) {
	ssize_t written;
	int size;
	const char * buffer = msg->serialize(size);

	CSubProcess::toStdin(buffer, size);
}



bool CMediaScannerCtrl::handleMsg(CMsgBase* msg) {
	bool rc = true;
	int type = msg->getType();
	switch(type) {
	    case E_MSG_ERR:
	    {
			CMsgError* error = reinterpret_cast<CMsgError*>(msg);
			cerr << "CMediaScannerCtrl::handleMsg Error: " << error->getMessage() << endl;
			m_parent->reportError(error->getID(), error->getErrorCode(), error->getMessage());
			break;
	    }
		case E_MSG_RESP:
		{
			CMsgResponse* reponse = reinterpret_cast<CMsgResponse*>(msg);
			m_parent->response(reponse->getRequestID(), reponse->getReturnCode(), reponse->getMessage());
			break;
		}
		case E_MSG_COLLECTION_CHANGED:
		{
			CMsgCollectionChanged* colChMsg = reinterpret_cast<CMsgCollectionChanged*>(msg);
			m_parent->collectionChanged(colChMsg->getNewRev(), 0, 0);
			break;
		}
		case E_MSG_PROGRESS:
		{
			CMsgProgress* progMsg = reinterpret_cast<CMsgProgress*>(msg);
			m_parent->scanProgress(progMsg->getJobID(), progMsg->getProgress());
			break;
		}
		case E_MSG_FINISHED:
		{
			CMsgFinished* finiMsg = reinterpret_cast<CMsgFinished*>(msg);
			m_parent->jobFinished(finiMsg->getJobID());
			break;
		}
		case E_MSG_QUIT:
		{
			CMsgQuit* quitMsg = reinterpret_cast<CMsgQuit*>(msg);
			rc = false;
			break;
		}

	}
	return rc;
}

}

