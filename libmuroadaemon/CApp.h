/*
 * CApp.h
 *
 *  Created on: 23 Oct 2011
 *      Author: martin
 */

#ifndef CAPP_H_
#define CAPP_H_

#include <mutex>
#include <log4cplus/logger.h>

#include "ConfixEx.h"
#include "CSettings.h"
#include "CAppenderErrorHandler.h"

namespace muroa {

class CApp {
private:
	CApp(int argc, char** argv) throw(configEx);

public:
	static CApp* getInstPtr(int argc = 0, char** argv = NULL) throw(configEx);
	virtual ~CApp();

	static CSettings& settings();
	static log4cplus::Logger& logger();

	CSettings& getSettingsRef();
	log4cplus::Logger& getLoggerRef();

	void serviceChanged();
	int daemonize();

	const boost::filesystem::path& getAbsProgDir() const {
		return m_abs_prog_dir;
	}

	const boost::filesystem::path& getCalledFromPath() const {
		return m_called_from_path;
	}

	const std::string& getProgName() const {
		return m_prog_name;
	}

private:
	static CApp* m_inst_ptr;
	static std::mutex m_mutex;

	CSettings m_settings;
	log4cplus::Logger m_logger;

	void initLog();

// to get rid of warning "auto_ptr is deprecated"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	std::auto_ptr<log4cplus::ErrorHandler> m_error_handler_ptr;
#pragma GCC diagnostic pop

    boost::filesystem::path m_abs_prog_dir;
    boost::filesystem::path m_called_from_path;
    std::string m_prog_name;
};

} /* namespace muroa */
#endif /* CAPP_H_ */
