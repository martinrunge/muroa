/***************************************************************************
 *
 *   main.cpp
 *
 *   This file is part of Asiotest                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/ndc.h>

#include "avahi/CAvahiThread.h"

#include <sys/stat.h>
#include <string.h>

#include "CSettings.h"
#include "CTcpServer.h"
#include "CSignalHandler.h"

#include <thread>

using namespace std;
using namespace log4cplus;

Logger logger = Logger::getInstance("main");
muroa::CSettings settings;

int main(int argc, char** argv) {

	SharedAppenderPtr logFileAppender(new FileAppender("myLogFile.log"));
	logFileAppender->setName("LogfileAppender");
	std::auto_ptr<Layout> myLayout = std::auto_ptr<Layout>(new log4cplus::TTCCLayout());
	logFileAppender->setLayout(myLayout);

	logger.addAppender(logFileAppender);
    // logger.setLogLevel ( DEBUG_LOG_LEVEL );
	logger.setLogLevel ( TRACE_LOG_LEVEL );

    BasicConfigurator config;
    config.configure();

    if(settings.parse(argc, argv) != 0) {
    	exit(0);
    }

    if(!settings.foreground()) {
    	errno = 0;
        if ( pid_t pid = fork() ) {
        	if (pid > 0) {
        		// in the parent process, exit.
        		//
        		exit(0);
        	}
        	else {
        		// pid < 0 -> failed to fork
        		LOG4CPLUS_ERROR(logger, "failed to fork" << strerror(errno) );
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
            		LOG4CPLUS_ERROR(logger, "second fork failed: " << strerror(errno) );
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
        		LOG4CPLUS_ERROR(logger, "Unable to open /dev/null: " << strerror(errno) );
        		return 1;
            }

            // Send standard output to a log file.
            const char* output = "/tmp/muroa.log";
            const int flags = O_WRONLY | O_CREAT | O_APPEND;
            const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            errno = 0;
            if (open(output, flags, mode) < 0)
            {
        		LOG4CPLUS_ERROR(logger, "Unable to open output file " << output << " :"<< strerror(errno) );
        		return 1;
            }

            // Also send standard error to the same log file.
            if (dup(1) < 0)
            {
        		LOG4CPLUS_ERROR(logger, "Unable to duplicate stdout" );
        		return 1;
            }

        }
    }


	try {

		boost::asio::io_service io_service;
		CTcpServer server(io_service);
		CSignalHandler::pointer sigPtr = CSignalHandler::create(io_service);
		sigPtr->start();
		CAvahiThread at(io_service);
		std::thread t(at);
		LOG4CPLUS_DEBUG(logger, "starting io_service");

		sleep(10);
		cerr << "trying to cacncel thread..." << endl;
		pthread_t nt = t.native_handle();
		pthread_cancel(nt);
		cerr << "called cancel on thread, joining ..." << endl;
		// t.join();
		cerr << "thread joined." << endl;

		io_service.run();
	} catch (std::exception& e) {
		LOG4CPLUS_ERROR(logger, "Uncaught exception from mainloop: " << e.what());
	}

	return 0;
}

