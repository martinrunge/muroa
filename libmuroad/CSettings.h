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

#include <string>

#include "Exceptions.h"

namespace muroa {

class CApp;

class CSettings {
public:
	CSettings(CApp* app) throw();
	virtual ~CSettings() throw();

	int parse(int argc, char** argv) throw(muroa::configEx);

	inline bool foreground() { return m_foreground; };
    inline int debuglevel() { return m_debuglevel; };
    unsigned short port();
    void setPort(unsigned short port);
    inline bool searchFreePort() { return m_search_free_port; };
    unsigned short ipversion() {return m_ip_version; };

    inline std::string logfile() { return m_logfile; };

    inline std::string serviceName() {return m_service_name; };
    inline std::string serviceType() {return m_service_type; };

    std::string getProptery(const std::string& key, const std::string& defaultVal);
    void setProptery(const std::string& key, const std::string& val);

    int getProptery(const std::string& key, const int& defaultVal);
    void setProptery(const std::string& key, const int& val);

private:
	void usage(std::string appname);
	void applyDefaults();

    std::string m_configfile;
    bool m_foreground;
    int m_debuglevel;
    unsigned short m_port;
    bool m_search_free_port;

    std::string m_service_name;
    std::string m_service_type;

    std::string m_logfile;

    unsigned short m_ip_version;
    boost::property_tree::ptree m_pt;

    CApp* m_app;
};

} /* namespace muroa */
#endif /* COPTARGS_H_ */
