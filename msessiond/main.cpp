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

#include <iostream>
#include <string>

#include <sys/stat.h>
#include <string.h>

#include <log4cplus/logger.h>

#include "CApp.h"
#include "CSettings.h"
#include "CSessionContainer.h"
#include "Exceptions.h"

#include "CConnection.h"

#include <boost/asio.hpp>

using namespace std;
using namespace log4cplus;
using namespace muroa;

int main(int argc, char** argv) {
	int rc = 0;

    muroa::CApp* app;
    muroa::CSessionContainer *sc;

    try {
    	app = muroa::CApp::getInstPtr(argc, argv);

    	if(!app->settings().foreground()) {
    		app->daemonize();
    	}

		boost::asio::io_service io_service;

		sc = CSessionContainer::create(io_service, app);

		LOG4CPLUS_DEBUG(app->logger(), "starting io_service");
		io_service.run();

    }
    catch( muroa::configEx ex ) {
    	cerr << ex.what() << endl;
    	rc = -1;
    }
    catch (std::exception& e) {
    	cerr << e.what() << endl;
		LOG4CPLUS_ERROR(app->logger(), "Uncaught exception from mainloop: " << e.what());
	}
    delete sc;
    delete app;

	return rc;
}

