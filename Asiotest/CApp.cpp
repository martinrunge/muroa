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

} /* namespace muroa */
