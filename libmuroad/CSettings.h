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

#include <string>

#include "Exceptions.h"

namespace muroa {

class CSettings {
public:
	CSettings() throw();
	virtual ~CSettings() throw();

	int parse(int argc, char** argv) throw(configEx);

	inline bool foreground() { return m_foreground; };
    inline int debuglevel() { return m_debuglevel; };
    inline unsigned port() { return m_port; };
    inline std::string logfile() { return m_logfile; };

private:
	void usage(std::string appname);
	void applyDefaults();

    std::string m_configfile;
    bool m_foreground;
    int m_debuglevel;
    unsigned m_port;

    std::string m_logfile;

};

} /* namespace muroa */
#endif /* COPTARGS_H_ */
