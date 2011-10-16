/*
 * CAvahiThread.h
 *
 *  Created on: 11 Oct 2011
 *      Author: martin
 */

#ifndef CAVAHITHREAD_H_
#define CAVAHITHREAD_H_

#include <boost/asio.hpp>
#include <thread>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

namespace muroa {

class CAvahiThread {
public:
	CAvahiThread( boost::asio::io_service& io_service );
	virtual ~CAvahiThread();

	void operator()() const;
	void cancel();

private:

	boost::asio::io_service& m_io_service;

	AvahiEntryGroup *m_group;
	AvahiSimplePoll *m_simple_poll;

};

}
#endif /* CAVAHITHREAD_H_ */
