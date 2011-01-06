/*
 * CMediaScannerCtrl.cpp
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#include "CMediaScannerCtrl.h"

#include "CMuroaServer.h"

#include <signal.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>


void sigusr1_handler(int signum) {
	// fprintf(stderr, "sigusr1 handler!!!!!\n");
	return;
}

using namespace std;

CMediaScannerCtrl::CMediaScannerCtrl(CMuroaServer *parent) : m_thread(0),
		                                                     m_run_message_loop(false),
		                                                     m_child_running(false),
		                                                     m_parent(parent) {

	struct sigaction new_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = sigusr1_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    int retval = sigaction(SIGUSR1, &new_action, &m_old_action);
    if(retval != 0)
    	perror("sigaction");
}

CMediaScannerCtrl::~CMediaScannerCtrl() {
    sigaction (SIGUSR1, &m_old_action, NULL);
}

void CMediaScannerCtrl::sendEvent(CMsgBase *msg) {
	ssize_t written;
	int size;
	const char * buffer = msg->serialize(size);

	written = write(m_socket, buffer, size);
	if(written != size) {
		cerr << "CMediaScannerCtrl::postEvent: sending of buffer incomplete!" << endl;
	}

}


void CMediaScannerCtrl::start() {

	if(m_run_message_loop) return;

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

	m_pid = m_subProcess.start("./build/mediascanner/mediascanner" , args ,0 ,0);
	m_child_running = true;
	m_waitthread = new thread( &CMediaScannerCtrl::waitPid, this);


	m_run_message_loop = true;
	m_thread = new thread( &CMediaScannerCtrl::loop, this);

}

void CMediaScannerCtrl::stop() {
	CMsgQuit quitmsg;
	int size;
	char *buffer = quitmsg.serialize(size);
	send(m_socket, buffer, size, 0);
	terminate();
}

void CMediaScannerCtrl::terminate() {
	m_run_message_loop = false;

	std::unique_lock<std::mutex> lk(m_mutex);
	int tries = 0;
	while(m_child_running) {
		bool signaled = m_cond_var.wait_for(lk, std::chrono::milliseconds(100));

		cerr << "m_cond_var.wait_for returned due to ";
		if(signaled) {
			cerr << "notification from waitpid -> mediascanner process exited." << endl;
		}
		else {
			switch (tries) {
				case 0:
				case 1:
					cerr << "timeout -> send SIGTERM to mediascanner process" << endl;
					kill(m_pid, SIGTERM);
					break;
				case 2:
				default:
					cerr << "timeout -> send SIGKILL to mediascanner process" << endl;
					kill(m_pid, SIGKILL);
			}
		}
		tries++;
	}

	// interrupt blocking recv in loop() out
	pthread_kill(m_thread->native_handle(), SIGUSR1);
	if(m_thread != 0) {
		m_thread->join();
		delete m_thread;
		m_thread = 0;
	}

	if(m_waitthread != 0) {
		m_waitthread->join();
		delete m_waitthread;
		m_waitthread = 0;
	}
}


void CMediaScannerCtrl::loop() {
	int bytesRecv;
	char buffer[257];

	while(m_run_message_loop) {
		errno = 0;
		bytesRecv = recv(m_socket, buffer, 256, 0);
		if(bytesRecv == -1 && errno == EINTR ) {
			// recv was interrupted by a signal, restart it if m_run_message_loop was not set to false, too.
			continue;
		}
		buffer[bytesRecv] = 0;
		cout << "recv: " << buffer << endl;

		CMsgBase* msg = CMsgBase::msgFactory( buffer, bytesRecv );
		handleMsg(msg);
	}
}


int CMediaScannerCtrl::handleMsg(CMsgBase* msg) {
	int rc = 0;
	int type = msg->getType();
	switch(type) {
		case E_MSG_RESP:
		{
			CMsgResponse* reponse = reinterpret_cast<CMsgResponse*>(msg);
			m_parent->response(reponse->getRequestID(), reponse->getReturnCode(), reponse->getMessage());
			rc = 0;
			break;
		}
		case E_MSG_COLLECTION_CHANGED:
		{
			rc = 0;
			break;
		}
		case E_MSG_PROGRESS:
		{
			CMsgProgress* progMsg = reinterpret_cast<CMsgProgress*>(msg);
			m_parent->scanProgress(progMsg->getProgress());
			rc = 0;
			break;
		}
	}
	return rc;
}


void CMediaScannerCtrl::waitPid() {
	int status;
	cerr << "waiting for process to exit" << endl;

	pid_t retval = -1;
	while(retval == -1) {
		errno = 0;
		retval = waitpid(m_pid, &status, 0);
		if(retval == -1) {
			cerr << "waitpid returned -1: " << endl;
			cerr << strerror(errno) << endl;
		}
		if(retval > 0) {
		    std::lock_guard<std::mutex> lk(m_mutex);
			cerr << "process " << retval << " changed state. Was waiting for " << m_pid << endl;
			cerr << strerror(errno) << endl;
			m_child_running = false;
		    m_cond_var.notify_one();
		}
	}
	cerr << "mediascanner exited. Status: " << status << endl;
}

