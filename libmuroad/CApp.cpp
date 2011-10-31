/*
 * CApp.cpp
 *
 *  Created on: 23 Oct 2011
 *      Author: martin
 */

#include "CApp.h"

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/ndc.h>

#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <string.h>

namespace muroa {

using namespace log4cplus;
using namespace std;

CApp* CApp::m_inst_ptr = 0;
std::mutex CApp::m_mutex;


CSettings& CApp::settings() { return m_settings; }
Logger& CApp::logger() { return m_logger; };


CApp::CApp(int argc, char** argv) throw(configEx) : m_logger(Logger::getInstance("main"))
{
	if( m_settings.parse(argc, argv) != 0) {
    	throw configEx("error parsing commandline parameters");
    }

	initLog();
}

CApp::~CApp() {}

CApp* CApp::getInstPtr(int argc, char** argv) throw(configEx) {
	lock_guard<mutex> lk(m_mutex);
	if( m_inst_ptr == 0) {
		m_inst_ptr = new CApp(argc, argv);
	}
	return m_inst_ptr;
}

void CApp::serviceChanged() {
	std::cerr << "CApp::serviceChanged()" << std::endl;
}

void CApp::initLog() {
	SharedAppenderPtr logFileAppender(new FileAppender(m_settings.logfile()));
	logFileAppender->setName("LogfileAppender");
	std::auto_ptr<Layout> myLayout = std::auto_ptr<Layout>(new log4cplus::TTCCLayout());
	logFileAppender->setLayout(myLayout);
	m_logger.addAppender(logFileAppender);
    // logger.setLogLevel ( DEBUG_LOG_LEVEL );
	m_logger.setLogLevel ( m_settings.debuglevel() );
}

int CApp::daemonize() {
	errno = 0;
    if ( pid_t pid = fork() ) {
    	if (pid > 0) {
    		// in the parent process, exit.
    		//
    		exit(0);
    	}
    	else {
    		// pid < 0 -> failed to fork
    		LOG4CPLUS_ERROR(logger(), "failed to fork" << strerror(errno) );
    		return 1;
    	}
    }
    else {
    	// in the child process
        // Make the process a new session leader. This detaches it from the
        // terminal.
        setsid();

        // A process inherits its working directory from its parent. This could be
        // on a mounted filesystem, which means that the running daemon would
        // prevent this filesystem from being unmounted. Changing to the root
        // directory avoids this problem.
        chdir("/");

        // The file mode creation mask is also inherited from the parent process.
        // We don't want to restrict the permissions on files created by the
        // daemon, so the mask is cleared.
        umask(0);

        // A second fork ensures the process cannot acquire a controlling terminal.
        errno = 0;
        if (pid_t pid = fork()) {
        	if (pid > 0) {
        		exit(0);
        	}
        	else {
        		LOG4CPLUS_ERROR(logger(), "second fork failed: " << strerror(errno) );
        		return 1;
        	}
        }

        // Close the standard streams. This decouples the daemon from the terminal
        // that started it.
        close(0);
        close(1);
        close(2);

        // We don't want the daemon to have any standard input.
        errno = 0;
        if (open("/dev/null", O_RDONLY) < 0)
        {
    		LOG4CPLUS_ERROR(logger(), "Unable to open /dev/null: " << strerror(errno) );
    		return 1;
        }

        // Send standard output to a log file.
        const char* output = "/tmp/muroa.log";
        const int flags = O_WRONLY | O_CREAT | O_APPEND;
        const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        errno = 0;
        if (open(output, flags, mode) < 0)
        {
    		LOG4CPLUS_ERROR(logger(), "Unable to open output file " << output << " :"<< strerror(errno) );
    		return 1;
        }

        // Also send standard error to the same log file.
        if (dup(1) < 0)
        {
    		LOG4CPLUS_ERROR(logger(), "Unable to duplicate stdout" );
    		return 1;
        }
    }

}


} /* namespace muroa */
