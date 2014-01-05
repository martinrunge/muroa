#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/ndc.h>

using namespace log4cplus;

Logger logger1;

class CLog {
public:
  CLog(Logger& l);
  ~CLog();
  
  Logger& getLoggerRef();
private:
  Logger m_logger;
};

CLog::CLog(Logger& l) : m_logger(std::move(l)) {
    LOG4CPLUS_DEBUG( m_logger, "test");
}

CLog::~CLog() {
}

Logger& CLog::getLoggerRef() {
  return m_logger;
}

int main(int argc, char* argv[]){
    PropertyConfigurator::doConfigure("log.properties");
    BasicConfigurator config;
    config.configure();

    logger1 = Logger::getInstance("main");

    CLog l(logger1);
    
    LOG4CPLUS_DEBUG( logger1, "test");
    
    Logger logger2 = l.getLoggerRef();
    LOG4CPLUS_DEBUG( logger2, "test");
    return 0;
}


