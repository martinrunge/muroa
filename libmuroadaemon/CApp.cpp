/*
 * CApp.cpp
 *
 *  Created on: 23 Oct 2011
 *      Author: martin
 */

#include "CApp.h"

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/ndc.h>

#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <string.h>
#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>

namespace muroa {

using namespace std;
using namespace log4cplus;

namespace bfs = boost::filesystem;

CApp* CApp::m_inst_ptr = 0;
std::mutex CApp::m_mutex;


CSettings& CApp::settings() { return CApp::getInstPtr()->m_settings; }
Logger& CApp::logger() { return CApp::getInstPtr()->m_logger; };

CSettings& CApp::getSettingsRef() { return m_settings; }
log4cplus::Logger& CApp::getLoggerRef() { return m_logger; }

CApp::CApp(int argc, char** argv, std::string config_file_basename) : m_settings(this)    // might throw configEx
{
    m_called_from_path = bfs::current_path();

    bfs::path abs_prog_path(argv[0]);
    if( abs_prog_path.is_relative()) {
    	abs_prog_path = m_called_from_path / abs_prog_path;
    }

    m_prog_name = abs_prog_path.filename().string();
    abs_prog_path.remove_filename();
    m_abs_prog_dir = bfs::canonical(abs_prog_path);

    if( m_settings.parse(argc, argv) != 0) {
    	throw configEx("error parsing commandline parameters");
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"   // to get rid of warning "auto_ptr is deprecated"

    m_error_handler_ptr = auto_ptr<log4cplus::ErrorHandler>(new CAppenderErrorHandler);

    #pragma GCC diagnostic pop

    initLog();

    string config_fn = config_file_basename;
    config_fn.append(".conf");
    string cache_fn = config_file_basename;
    cache_fn.append(".cache");
    
    
	m_settings.pushConfigFilePath(m_abs_prog_dir/config_fn);
	m_settings.pushConfigFilePath(m_abs_prog_dir/"etc"/config_fn);
	m_settings.pushConfigFilePath(bfs::path("/etc")/config_fn);

	m_settings.pushPersistFilePath(m_abs_prog_dir/cache_fn);
	m_settings.pushPersistFilePath(m_abs_prog_dir/"var/cache"/cache_fn);
	m_settings.pushPersistFilePath(bfs::path("/var/cache")/cache_fn);

    m_settings.readConfigFile();
    m_settings.readCacheFile();
}

CApp::~CApp() {}

CApp* CApp::getInstPtr(int argc, char** argv, std::string config_file_basename) {   // might throw configEx
	lock_guard<mutex> lk(m_mutex);
	if( m_inst_ptr == 0) {
		m_inst_ptr = new CApp(argc, argv, config_file_basename);
	}
	return m_inst_ptr;
}

void CApp::serviceChanged() {
	std::cerr << "CApp::serviceChanged()" << std::endl;
}

void CApp::initLog() {
    try
    {
        log4cplus::PropertyConfigurator::doConfigure("log.properties");
    }
    catch( ... )
    {
       std::cerr<<"Exception occured while opening log.properties\n";
       BasicConfigurator config;
       config.configure();
    }

    m_logger = Logger::getInstance("main");
    Appender* appender;
    bool logfile_accessible = CSettings::accessible(m_settings.logfile());
    if( logfile_accessible ) {
        appender = new FileAppender(m_settings.logfile());
        appender->setErrorHandler(std::move(m_error_handler_ptr));
    }
    else {
    	appender = new ConsoleAppender();
    	appender->setErrorHandler(std::move(m_error_handler_ptr));
        //SharedAppenderPtr log_appender(console_appender);

    }
    SharedAppenderPtr log_appender(appender);
	log_appender->setName("LogAppender");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"   // to get rid of warning "auto_ptr is deprecated"

	std::unique_ptr<Layout> myLayout = std::auto_ptr<Layout>(new log4cplus::PatternLayout("%d{%H:%M:%S,%q} [ %t: %-5p ] %m%n"));

#pragma GCC diagnostic pop

	log_appender->setLayout(std::move(myLayout));

	m_logger.addAppender(log_appender);
    // logger.setLogLevel ( DEBUG_LOG_LEVEL );
	m_logger.setLogLevel ( m_settings.debuglevel() );
    if( !logfile_accessible ) {
    	LOG4CPLUS_WARN(m_logger, "Could not open logfile '" << m_settings.logfile() << "'. Logging to console instead." << endl
    			                  << "Pass '--logfile </path/to/file.log>' to specify a logfile writable by '" << m_prog_name << "'.");
    }
}



int CApp::daemonize() {
	errno = 0;
    if ( pid_t pid = fork() ) {
    	if (pid > 0) {
    		// in the parent process, exit.
    		//
    		LOG4CPLUS_INFO(logger(), "forking to background ...");
    		cout << "forking to background ..." << endl;
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
    return 0;

}

} /* namespace muroa */

#ifdef MUROA_FAKE_LOG4CPLUS_RVALREF
namespace log4cplus {
Logger::Logger (Logger && rhs)
{
    value = rhs.value;
    rhs.value = 0;
}


Logger &
Logger::operator = (Logger && rhs)
{
    Logger (std::move (rhs)).swap (*this);
    return *this;
}
}
#endif

