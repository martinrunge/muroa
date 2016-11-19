/***************************************************************************
 *
 *   COptArgs.h
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

#ifndef COPTARGS_H_
#define COPTARGS_H_

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "boost/filesystem/path.hpp"

#include <string>
#include <stack>

#include "Exceptions.h"

namespace muroa {

class CApp;

class CSettings {
public:
	CSettings(CApp* app) throw();
	virtual ~CSettings() throw();

	void pushConfigFilePath(boost::filesystem::path cf);
	void pushPersistFilePath(boost::filesystem::path cf);

	int parse(int argc, char** argv) throw(muroa::configEx);
	int readConfigFile() throw(muroa::configEx);
	int readCacheFile() throw(muroa::configEx);
	int resetCache();


	inline bool foreground() { return m_foreground; };
    inline int debuglevel() { return m_debuglevel; };
    unsigned short port();
    void setPort(unsigned short port);
    inline bool searchFreePort() { return m_search_free_port; };
    unsigned short ipversion() {return m_ip_version; };

    unsigned short timeserver_port() { return m_ts_port; };
    std::vector<std::string> muroad_addrs() { return m_muroad; };

    inline std::string logfile() { return m_logfile; };

    inline std::string serviceName() {return m_service_name; };
    inline void setServiceName(std::string service_name ) { m_service_name = service_name; };

    inline std::string serviceType() {return m_service_type; };
    inline void setServiceType(std::string service_type) { m_service_type = service_type; };

    std::string getConfigVal(const std::string& key, const char* defaultVal);
    std::string getConfigVal(const std::string& key, const std::string& defaultVal);
    int getConfigVal(const std::string& key, const int& defaultVal);
    bool getConfigVal(const std::string& key, const bool& defaultVal);

    std::string getPersisentVal(const std::string& key, const char* defaultVal);
    void setPersistentVal(const std::string& key, const int& val);

    std::string getPersisentVal(const std::string& key, const std::string& defaultVal);
    void setPersistentVal(const std::string& key, const std::string& val);

    std::vector<std::string> getPersisentVal(const std::string& prent_key, const std::vector<std::string>& defaultVal);
    void setPersistentVal(const std::string& parent_key, const std::vector<std::string>& val);

    int getPersisentVal(const std::string& key, const int& defaultVal);
    void setPersistentVal(const std::string& key, const char* val);

    bool getPersisentVal(const std::string& key, const bool& defaultVal);
    void setPersistentVal(const std::string& key, const bool& val);

    static bool accessible(std::string filename);

private:
	void usage(std::string appname);
	void applyDefaults();

	void createMinimalJsonFile(std::string filename);

    std::string m_configfile;
    std::string m_logfile;
    // cachefile: usually in /var/cache, used to hold internal runtime values persistent over
    // restart, e.g. the measured time it took to restart an audio stream on this perticular client
    std::string m_cachefile;

    bool m_foreground;
    int m_debuglevel;
    unsigned short m_port;
    bool m_search_free_port;
    bool m_reset_cache;

    // address of muroad for manual setting for testing
    std::vector<std::string> m_muroad;
    unsigned short m_ts_port;


    std::string m_service_name;
    std::string m_service_type;


    unsigned short m_ip_version;
    boost::property_tree::ptree m_config_pt;
    boost::property_tree::ptree m_persist_pt;

    std::stack<std::string> m_search_config_file;
    std::stack<std::string> m_search_persist_file;

    CApp* m_app;
};

} /* namespace muroa */
#endif /* COPTARGS_H_ */
