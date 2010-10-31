/*
 * CMediaScannerCtrl.cpp
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#include "CMediaScannerCtrl.h"

#include <signal.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>


void sigchild_handler(int signum) {
	fprintf(stderr, "signchild handler!!!!!\n");
	int* nullptr = 0;
	int val = *nullptr;
}

using namespace std;

CMediaScannerCtrl::CMediaScannerCtrl() : m_thread(0),
		                                 m_running(false) {

	struct sigaction new_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = sigchild_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    int retval = sigaction(SIGCHLD, &new_action, &m_old_action);
    if(retval != 0)
    	perror("sigaction");
}

CMediaScannerCtrl::~CMediaScannerCtrl() {
    sigaction (SIGCHLD, &m_old_action, NULL);
}


void CMediaScannerCtrl::start() {

	if(m_running) return;

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
	m_waitthread = new thread( &CMediaScannerCtrl::waitPid, this);


	m_running = true;
	m_thread = new thread( &CMediaScannerCtrl::loop, this);

	send(m_socket, "hallo", 5, 0);
	sleep(1);
	send(m_socket, "hallo", 5, 0);
	sleep(1);
	send(m_socket, "hallo", 5, 0);

}


void CMediaScannerCtrl::stop() {
	m_running = false;

	if(m_thread != 0) {
		m_thread->join();
		delete m_thread;
		m_thread = 0;
	}

}


void CMediaScannerCtrl::loop() {
	int bytesRecv;
	char buffer[257];

	while(m_running) {
		bytesRecv = recv(m_socket, buffer, 256, 0);
		buffer[bytesRecv] = 0;
		cout << "recv: " << buffer << endl;
	}
}

void CMediaScannerCtrl::waitPid() {
	int status;
	cerr << "waiting for process to exit" << endl;
	waitpid(m_pid, &status, 0);
	cerr << "mediascanner exited. Status: " << status << endl;
}

