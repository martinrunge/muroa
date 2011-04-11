/*
 * CSubProcess.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CSubProcess.h"

#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

CSubProcess::CSubProcess() {
}

CSubProcess::~CSubProcess() {
}


pid_t CSubProcess::start(std::string executable, std::vector<std::string> args, int cpu_prio, int io_prio) {

	pid_t child_pid = fork();
	if(child_pid == 0) {
		// in child process
		const char ** argv;
		argv = new (const char * [args.size() + 2]);

		argv[0] = executable.c_str();
		for(int i = 0; i < args.size(); i++) {
			argv[i + 1] = args[i].c_str();
		}
		argv[ args.size() + 1] = NULL;

		int retval = execve(executable.c_str(), (char* const*)argv, NULL);
		m_last_error_code = retval;
		m_last_error_msg = strerror(errno);

	}
	else {
		if(child_pid == -1) {
			// error
			cerr << "CSubProcess::start(" << executable << ", "<<  ") failed: " << strerror(errno) << endl;
		}
		else {
			// ok child was created
		}
	}
	return child_pid;
}

int CSubProcess::waitForExit(int timeout_in_ms) {
	return 0;
}

int CSubProcess::sendSignal(int sig_no) {
	return 0;
}

int CSubProcess::read(char* buffer, int length) {
	return 0;
}

int CSubProcess::write(char* buffer, int length) {
	return 0;
}

