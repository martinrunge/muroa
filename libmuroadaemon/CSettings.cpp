/***************************************************************************
 *
 *   COptArgs.cpp
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
 *   Free Software Foundation, Inc.
/usr/include/boost/property_tree/detail/ptree_implementation.hpp:362:9: note: ,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "CSettings.h"
#include "CApp.h"

#include <getopt.h>
#include <log4cplus/loggingmacros.h>

#include "boost/filesystem/operations.hpp"
#include <boost/system/error_code.hpp>

using namespace std;
using namespace log4cplus;

namespace fs = boost::filesystem;

namespace muroa {

namespace bfs = boost::filesystem;

CSettings::CSettings(CApp* app) throw() : m_foreground(false),
		                                  m_search_free_port(false),
                                          m_debuglevel(0),
                                          m_logfile(""),
                                          m_app(app),
                                          m_reset_cache(false)
{

    applyDefaults();
}


int CSettings::parse(int argc, char** argv) throw(configEx) {
    int c;
    int digit_optind = 0;

    static struct option long_options[] = {
    	{"cachefile", 1, 0, 'a'},
        {"configfile", 1, 0, 'c'},
        {"foreground", 0, 0, 'f'},
        {"debuglevel", 1, 0, 'd'},
        {"port", 1, 0, 'p'},
        {"searchport", 0, 0, 's'},
        {"logfile", 1, 0, 'l'},
        {"reset", 0, 0, 'r'},
        {"help", 0, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long(argc, argv, "a:c:fsl:p:r?", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'a':
            if (optarg) {
                m_cachefile = optarg;
            }
            else {
            	throw configEx("--cachefile option requires an argument");
            }
            break;

        case 'c':
            if (optarg) {
                m_configfile = optarg;
            }
            else {
            	throw configEx("--configfile option requires an argument");
            }
            break;

        case 'd':
            m_debuglevel = strtoul(optarg, NULL, 10);
            break;

        case 'f':
        	m_foreground = true;
        	break;

        case 'p':
            m_port = strtoul(optarg, NULL, 10);
            break;

        case 'r':
            m_reset_cache = true;
            break;

        case 's':
            m_search_free_port = true;
            break;

        case 'l':
            if (optarg) {
                m_logfile = optarg;
            }
            else {
            	throw configEx("--logfile option requires an argument");
            }
            break;

        case '?':
        	usage(argv[0]);
        	return 1;
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
            break;
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
//    using boost::property_tree::ptree;
//    try {
//        // beware of boost bug resolved in this changeset: https://svn.boost.org/trac/boost/changeset/78679
//        // came up with c++11
//    	read_json(m_configfile, m_pt);
//    }
//    catch(boost::property_tree::json_parser::json_parser_error& err) {
//    	LOG4CPLUS_ERROR( m_app->getLoggerRef() , "failed to load config file '" << m_configfile << "': " << err.what() );
//    }
    return 0;
}

int CSettings::readConfigFile() throw(muroa::configEx) {
    using boost::property_tree::ptree;
    bool config_success = false;

    string current_config_file;
	if(!m_configfile.empty()) {
		// a config file was given as program argument -> try it and none else.
		while(!m_search_config_file.empty()) {
			m_search_config_file.pop();
		}
		m_search_config_file.push(m_configfile);
	}

    do {
    	try { // try to load the config file from the next possible location
    		current_config_file = m_search_config_file.top();
    		m_search_config_file.pop();
    		// beware of boost bug resolved in this changeset: https://svn.boost.org/trac/boost/changeset/78679
			// came up with c++11
			read_json(current_config_file, m_config_pt);
			// if no exception occurred ...
			config_success = true;

			bfs::path cur_path(current_config_file);
			m_configfile = bfs::absolute(cur_path).native();

			LOG4CPLUS_INFO( m_app->getLoggerRef() , "Successfully loaded config file '" << current_config_file << "'.");
			break;
		}
		catch(boost::property_tree::json_parser::json_parser_error& err) {
			LOG4CPLUS_INFO( m_app->getLoggerRef() , "failed to load config file '" << current_config_file << "': " << err.what());
		}
    }
	while(!m_search_config_file.empty());

    if( config_success == false) {
    	ostringstream oss;
    	if(m_configfile.empty()) {
    		oss << "Config file not found at any of its default locations." << endl
    			<< " please use '--configfile </path/to/configfile>' to pass a valid config file to '" << m_app->getProgName() << "'";
    	}
    	else {
    		oss << "Given config file '" << m_configfile << "' could not be read.";
    	}
		LOG4CPLUS_ERROR( m_app->getLoggerRef() , oss );
    	throw configEx(oss.str());
    }

    return 0;
}

int CSettings::resetCache() {
    bool cache_success = false;
    std::stack<std::string> copy_of_search_cache_file;
    string current_cache_file;

    LOG4CPLUS_INFO( m_app->getLoggerRef(), "Resetting persistent runtime cache ...");

	if(!m_cachefile.empty()) {
		copy_of_search_cache_file.push(m_cachefile);
	}
	else {
		copy_of_search_cache_file = m_search_persist_file;
	}

	while(!copy_of_search_cache_file.empty()) {
		current_cache_file = copy_of_search_cache_file.top();
		copy_of_search_cache_file.pop();

		bfs::path cf(current_cache_file);
		if(bfs::is_regular(cf)) {
			bool success = bfs::remove(cf);
			if(success) {
				LOG4CPLUS_INFO( m_app->getLoggerRef(), "	removed '" << current_cache_file << "'");
				write_json(current_cache_file, m_persist_pt);
			}
			else {
				LOG4CPLUS_WARN( m_app->getLoggerRef(), "	could not remove '" << current_cache_file << "'");
			}
		}
	}
}

int CSettings::readCacheFile() throw(muroa::configEx) {
    using boost::property_tree::ptree;
    bool cache_success = false;

    string current_cache_file;
	if(!m_cachefile.empty()) {
		// a cache file was given as program argument -> try it and none else.
		while(!m_search_persist_file.empty()) {
			m_search_persist_file.pop();
		}
		m_search_persist_file.push(m_cachefile);
	}

	try { // try the next possible cache file location (it may not be present yet)
		while(!m_search_persist_file.empty()) {
    		current_cache_file = m_search_persist_file.top();
    		m_search_persist_file.pop();
    		// beware of boost bug resolved in this changeset: https://svn.boost.org/trac/boost/changeset/78679
			// came up with c++11
    		if(accessible(current_cache_file)) {

    			read_json(current_cache_file, m_persist_pt);
    			// if no exception occurred ...
    			cache_success = true;
    			bfs::path cur_path(current_cache_file);
    			m_cachefile = bfs::absolute(cur_path).native();
    			LOG4CPLUS_INFO( m_app->getLoggerRef(), "Successfully restored persistent runtime values from '" << current_cache_file << "'.");
    			break;
    		}
		}
    }
	catch(boost::property_tree::json_parser::json_parser_error& err) {
		LOG4CPLUS_INFO( m_app->getLoggerRef() , "Could not restore persistent runtime values from '" << current_cache_file << "': " << err.what() << endl <<" Using default values.");
		// current_cache_file was accessible, but could not be parsed. Reset it.
		m_cachefile = current_cache_file;
		resetCache();
	}

    return 0;
}

unsigned short CSettings::port() {
	return m_port;
};

void CSettings::setPort(unsigned short port) {
	m_port = port;
}
/***
 *  there is an built-in conversion from const char* to bool, that will win over const char* to std::string,
 *  because std::string is not built-in but user-defined. Without the following to overloads, calling
 *  getProperty(key, "myvalue") with the default value hardcoded will return a bool instead of the desired std::string.
 */
string CSettings::getConfigVal(const string& key, const char* defaultVal) {
	return getConfigVal(key, string(defaultVal));
}

string CSettings::getConfigVal(const string& key, const string& defaultVal) {
	return m_config_pt.get(key, defaultVal);
}

int CSettings::getConfigVal(const string& key, const int& defaultVal) {
	return m_config_pt.get(key, defaultVal);
}

bool CSettings::getConfigVal(const std::string& key, const bool& defaultVal) {
	return m_config_pt.get(key, defaultVal);
}


string CSettings::getPersisentVal(const string& key, const char* defaultVal) {
	return getPersisentVal(key, string(defaultVal));
}

void CSettings::setPersistentVal(const std::string& key, const char* val) {
	setPersistentVal(key, string(val));
}

string CSettings::getPersisentVal(const string& key, const string& defaultVal) {
	return m_persist_pt.get(key, defaultVal);
}

void CSettings::setPersistentVal(const string& key, const string& val) {
	m_persist_pt.put(key, val);
	write_json(m_cachefile, m_persist_pt);
}

int CSettings::getPersisentVal(const std::string& key, const int& defaultVal) {
	return m_persist_pt.get(key, defaultVal);
}

void CSettings::setPersistentVal(const string& key, const int& val) {
	m_persist_pt.put(key, val);
	write_json(m_cachefile, m_persist_pt);
}

bool CSettings::getPersisentVal(const std::string& key, const bool& defaultVal) {
	return m_persist_pt.get(key, defaultVal);
}

void CSettings::setPersistentVal(const std::string& key, const bool& val) {
	m_persist_pt.put(key, val);
	write_json(m_cachefile, m_persist_pt);
}


CSettings::~CSettings() throw() {

}

void CSettings::usage(string appname) {
	cout << "usage:" << endl;
	cout << appname << " <options>" << endl;
	cout << "  --configfile  -c  \tspecify a config file" << endl;
	cout << "  --logfile     -l  \tspecify a logfile" << endl;
	cout << "  --cachefile   -a  \tspecify a cachefile (persistent storage of runtime values)" << endl;
	cout << "  --reset-cache -r  \treset all runtime values to their default" << endl;
	cout << "  --port        -p  \tlisten on port" << endl;
	cout << "  --searchport  -s  \tsearch for a free port, starting at --port" << endl;
	cout << "  --foreground  -f  \tdo not fork into background, do not become a daemon, log to stderr." << endl;
	cout << "  --debuglevel  -d  \tdebuglevel" << endl;
	cout << "  --help        -?  \tthis message" << endl;
}

void CSettings::applyDefaults() {
	m_logfile = "/var/log/muroad.log";

	m_search_config_file.push("muroa.conf");
	m_search_config_file.push("etc/muroa.conf");
	m_search_config_file.push("/etc/muroa.conf");

	m_search_persist_file.push("muroa.cache");
	m_search_persist_file.push("var/cache/muroa.cache");
	m_search_persist_file.push("/var/cache/muroa.cache");

    m_service_name = "Muroa streaming client";
    m_service_type = "_muroad._udp";

    m_ip_version = 4;
}

bool CSettings::accessible(string filename) {
	bfs::path lf(filename);
	if(bfs::is_directory(lf)) {
		return false;
	}
	else {
		int rc = access(filename.c_str(), R_OK|W_OK);
		if(rc == 0) {
			return true;
		}
		else {
			switch (errno) {
			case EACCES:
			case EROFS:
				return false;

			case ENOENT:
			{
				// try to create parent path, if not yet there:
				boost::system::error_code ec;
				create_directories(lf.parent_path(), ec);
				if( ec ) {
					// could not create path to logfile
					return false;
				}
				int wr_fd = ::open(filename.c_str(), O_RDWR|O_CREAT|FD_CLOEXEC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
				if(wr_fd == -1) {
					return false;
				}
				else {
					close(wr_fd);
					bfs::remove(lf);
					return true;
				}
			}
			default:
				return false;
			}
		}
	}
	return false;
}

void CSettings::createMinimalJsonFile(string filename) {
	bfs::path pf(filename);
}



} /* namespace muroa */
