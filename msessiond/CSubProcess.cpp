/*
 * CSubProcess.cpp
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#include "CSubProcess.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;
using namespace boost;
using namespace boost::asio;

CSubProcess::CSubProcess(io_service& io_service) : m_io_service(io_service),
                                                   m_stdin_sock(io_service),
                                                   m_stdout_sock(io_service),
		                                           m_stderr_sock(io_service),
		                                           m_waitthread(0),
		                                           m_running(false)
{
}

CSubProcess::~CSubProcess() {
}


pid_t CSubProcess::start(std::string executable, std::vector<std::string> args, int cpu_prio, int io_prio) {

	if(m_running || m_pid != 0) {
		// already running.
		cerr << "CSubProcess::start(" << executable << ", "<<  ") failed: already running a subprocess." << endl;
		return -1;
	}

	// redirect stdin, stdout and stderr
	int retval;
	int stdin_sds[2];    // pipefds[0] : read    pipefds[1]: write
	int stdout_sds[2];    // pipefds[0] : read    pipefds[1]: write
	int stderr_sds[2];    // pipefds[0] : read    pipefds[1]: write

	// retval = pipe(stdin_pipefds);
	retval = ::socketpair(AF_UNIX, SOCK_STREAM, 0, stdin_sds);
	if(retval != 0) {
		::perror("pipe");
	}

	// retval = pipe(stdout_pipefds);
	retval = ::socketpair(AF_UNIX, SOCK_STREAM, 0, stdout_sds);
	if(retval != 0) {
		::perror("pipe");
	}

	// retval = pipe(stderr_pipefds);
	retval = ::socketpair(AF_UNIX, SOCK_STREAM, 0, stderr_sds);
	if(retval != 0) {
		::perror("pipe");
	}

	m_pid = fork();
	if(m_pid == 0) {
		// in child process
		const char ** argv;
		argv = new (const char * [args.size() + 2]);
		// cerr << "### in child process" << endl;

		argv[0] = executable.c_str();
		for(int i = 0; i < args.size(); i++) {
			argv[i + 1] = args[i].c_str();
		}
		argv[ args.size() + 1] = NULL;

		// cerr << "### dup-ing stdin ..." << endl;
		if( dup2( stdin_sds[0], fileno(stdin)) != STDIN_FILENO) {
			::perror("Cannot dup2 stdin");
		}
		// cerr << "### dup-ing stdout ..." << endl;
		if( dup2( stdout_sds[1], fileno(stdout)) != STDOUT_FILENO) {
			::perror("Cannot dup2 stdout");
		}
		// cerr << "### dup-ing stderr ..." << endl;
		if( dup2( stderr_sds[1], fileno(stderr)) != STDERR_FILENO) {
			::perror("Cannot dup2 stderr");
		}

		close(stdin_sds[1]);
		close(stdout_sds[0]);
		close(stderr_sds[0]);

		// cerr << "### now executing child process ..." << endl;
		retval = execve(executable.c_str(), (char* const*)argv, NULL);
		if(retval == -1) {
			cerr << "Error executing '" << executable << "' :" << ::strerror(errno) << endl;
			// sleep(10);
			::exit(EXIT_FAILURE);
		}

	}
	else {
		if(m_pid == -1) {
			// error
			cerr << "CSubProcess::start(" << executable << ", "<<  ") failed: " << ::strerror(errno) << endl;
		}
		else {
			m_stdin_sock.assign(boost::asio::local::stream_protocol(), stdin_sds[1]);
			m_stdout_sock.assign(boost::asio::local::stream_protocol(), stdout_sds[0]);
			m_stderr_sock.assign(boost::asio::local::stream_protocol(), stderr_sds[0]);

            close( stdin_sds[0] );
			close( stdout_sds[1] );
			close( stderr_sds[1] );

			m_running = true;
			watchStdout();
			watchStderr();
			m_waitthread = new thread( &CSubProcess::waitPid, this);
		}
	}
	return m_pid;
}

int CSubProcess::waitForExit(int timeout_in_ms) {
	return 0;
}

int CSubProcess::sendSignal(int sig_no) {
	return ::kill(m_pid, sig_no);
}

void CSubProcess::toStdin(const char* buffer, int length) {
	m_stdin_sock.async_send(asio::buffer(buffer, length),
                            boost::bind(&CSubProcess::writtenToStdin, this,
                            asio::placeholders::error,
                            asio::placeholders::bytes_transferred));
	return;

}

void CSubProcess::writtenToStdin(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
    }
}

void CSubProcess::watchStdout() {
	m_stdout_sock.async_read_some(asio::buffer(m_stdout_buffer),
                                  boost::bind(&CSubProcess::readFromStdout, this,
                                  asio::placeholders::error,
                                  asio::placeholders::bytes_transferred));
}

void CSubProcess::watchStderr() {
	m_stderr_sock.async_read_some(asio::buffer(m_stderr_buffer),
                                  boost::bind(&CSubProcess::readFromStderr, this,
                                  asio::placeholders::error,
                                  asio::placeholders::bytes_transferred));
}

void CSubProcess::readFromStdout(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
    	fromStdout(m_stdout_buffer.cbegin(), bytes_transferred);
    	watchStdout();
    }
    else {
    	cerr << error.message() << endl;
    	// delete this;
    }
}

void CSubProcess::readFromStderr(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
    	fromStderr(m_stderr_buffer.cbegin(), bytes_transferred);
    	watchStderr();
    }
    else {
    	cerr << error.message() << endl;
    	// delete this;
    }
}

void CSubProcess::fromStdout(const char* buffer, int length) {

}

void CSubProcess::fromStderr(const char* buffer, int length) {

}

void CSubProcess::finished() {
	m_waitthread->join();
	delete m_waitthread;
	m_waitthread = 0;
	// m_parent->child finished
}

void CSubProcess::waitPid() {
	int status;
	cerr << "waiting for process to exit" << endl;

	pid_t retval = -1;
	while(retval == -1) {
		errno = 0;
		cerr << "waitpid: waiting for pid:" << m_pid << endl;
		retval = waitpid(m_pid, &status, 0);
		if(retval == -1) {
			cerr << "waitpid returned -1: " << endl;
			cerr << ::strerror(errno) << endl;
		}
		if(retval > 0) {
			cerr << "process " << retval << " changed state. Was waiting for " << m_pid << endl;
			cerr << ::strerror(errno) << endl;
			m_running = false;
			m_pid = 0;

			m_io_service.post(boost::bind(&CSubProcess::finished, this));
		}
	}
	cerr << "subprocess exited. Status: " << status << endl;

	m_stdin_sock.close();
	m_stdout_sock.close();
	m_stderr_sock.close();

}
