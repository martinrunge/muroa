/***************************************************************************
 *
 *   CSignalHandler.cpp
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

#include "CSignalHandler.h"

#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <signal.h>

#include <iostream>

using namespace std;
using namespace boost;

int sighupsock;

void  SigHupHandler(int sig)
{
     char  c;
     fprintf(stderr, "SigHup %d\n", sig);
     write(sighupsock, &sig, sizeof(sig));
}


CSignalHandler::CSignalHandler(boost::asio::io_service& io_service) : m_socket(io_service){
	int sv[2];
	int retval = socketpair(AF_LOCAL, SOCK_STREAM, 0, sv);
	if(retval != 0) {
		perror("socketpair");
	}

	sighupsock = sv[0];
	m_socket.assign(boost::asio::local::stream_protocol(), sv[1]);

	installSigHandlers();
}

CSignalHandler::~CSignalHandler() {
	sigaction (SIGHUP, &m_old_sigaction, NULL);
}

void CSignalHandler::start() {
	m_socket.async_read_some(asio::buffer(m_buffer),
                             boost::bind(&CSignalHandler::handle_read, shared_from_this(),
                                         asio::placeholders::error,
                                         asio::placeholders::bytes_transferred));


}

void CSignalHandler::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
    	std::vector<char>::iterator it;
    	if(bytes_transferred > 0) {
 			std::cerr << m_buffer.data();
    	}
    	std::cerr << "CSignalHandler::handle_read(" << bytes_transferred << ")" << std::endl;
    	start();
    }
    else {
    	delete this;
    }
}



void CSignalHandler::installSigHandlers() {

	struct sigaction new_action;

	// Set up the structure to specify the new action.
	new_action.sa_handler = SigHupHandler;
	sigemptyset (&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction (SIGHUP, NULL, &m_old_sigaction);
	sigaction (SIGHUP, &new_action, NULL);

}
