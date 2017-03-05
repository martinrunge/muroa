/*
 * CAvahiThread.cpp
 *
 *  Created on: 11 Oct 2011
 *      Author: martin
 */

#include "CAvahiThread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <poll.h>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#include <iostream>
#include <sstream>

#include "ConfixEx.h"

using namespace std;

namespace muroa {

CAvahiThread::CAvahiThread( boost::asio::io_service& io_service ) : m_io_service(io_service),
		                                                            m_group(NULL),
		                                                            m_simple_poll(NULL)
{
    /* Allocate main loop object */
    if (!(m_simple_poll = avahi_simple_poll_new())) {
        ostringstream oss;
        oss << "Failed to create simple poll object." << endl;

        throw(configEx(oss.str()));
    }

}


CAvahiThread::~CAvahiThread() {
	if(m_simple_poll != NULL) {
        avahi_simple_poll_free(m_simple_poll);
        m_simple_poll = NULL;
	}
}

void CAvahiThread::operator()() const {
    AvahiClient *client = NULL;
	if(m_simple_poll != NULL) {
		/* Allocate a new client */

		client = avahi_client_new(avahi_simple_poll_get(m_simple_poll), 0, client_callback, NULL, &error);

		/* Check wether creating the client object succeeded */
		if (client) {

//			/* After 10s do some weird modification to the service */
//			avahi_simple_poll_get(m_simple_poll)->timeout_new(
//				avahi_simple_poll_get(m_simple_poll),
//				avahi_elapse_time(&tv, 1000*10, 0),
//				modify_callback,
//				client);

			/* Run the main loop */
			avahi_simple_poll_loop(m_simple_poll);
		}
	}
}

void CAvahiThread::cancel() {
	avahi_simple_poll_quit(m_simple_poll);
}

}
