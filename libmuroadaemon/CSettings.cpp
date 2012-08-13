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
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "CSettings.h"
#include "CApp.h"

#include <getopt.h>

using namespace std;
using namespace log4cplus;

namespace muroa {

CSettings::CSettings(CApp* app) throw() : m_foreground(false),
                                          m_debuglevel(0),
                                          m_logfile(""),
                                          m_app(app) {

    applyDefaults();

}


int CSettings::parse(int argc, char** argv) throw(configEx) {
    int c;
    int digit_optind = 0;

    static struct option long_options[] = {
        {"configfile", 1, 0, 'c'},
        {"foreground", 0, 0, 'f'},
        {"debuglevel", 1, 0, 'd'},
        {"port", 1, 0, 'p'},
        {"searchport", 0, 0, 's'},
        {"logfile", 1, 0, 'l'},
        {"help", 0, 0, '?'},
        {0, 0, 0, 0}
    };


    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long(argc, argv, "c:fl:p:?", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
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
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
    using boost::property_tree::ptree;
    try {
    	read_json(m_configfile, m_pt);
    }
    catch(boost::property_tree::json_parser::json_parser_error err) {
    	LOG4CPLUS_ERROR( m_app->logger() , "failed to load config file '" << m_configfile << "': " << err.what() );
    }
    return 0;

}

unsigned short CSettings::port() {
	return m_port;
};

void CSettings::setPort(unsigned short port) {
	m_port = port;
}

string CSettings::getProptery(const string& key, const string& defaultVal) {
	return m_pt.get(key, defaultVal);
}

void CSettings::setProptery(const string& key, const string& val) {
	m_pt.put(key, val);
}

int CSettings::getProptery(const string& key, const int& defaultVal) {
	return m_pt.get(key, defaultVal);
}

void CSettings::setProptery(const string& key, const int& val) {
	m_pt.put(key, val);
}

CSettings::~CSettings() throw() {

}

void CSettings::usage(string appname) {
	cout << "usage:" << endl;
	cout << appname << " <options>" << endl;
	cout << "  --configfile  -c  \tspecify a config file" << endl;
	cout << "  --logfile  -l     \tspecify a logfile" << endl;
	cout << "  --port        -p  \tlisten on port" << endl;
	cout << "  --searchport  -s  \tsearch for a free port, starting at --port" << endl;
	cout << "  --foreground  -f  \tdo not fork into background, do not become a daemon, log to stderr." << endl;
	cout << "  --debuglevel  -d  \tdebuglevel" << endl;
	cout << "  --help        -?  \tthis message" << endl;
}

void CSettings::applyDefaults() {
	m_logfile = "/var/log/muroad.log";
	m_configfile = "/etc/muroa.conf";

    m_service_name = "MuroaClient";
    m_service_type = "_muroa._tcp";

    m_ip_version = 4;
}



} /* namespace muroa */
