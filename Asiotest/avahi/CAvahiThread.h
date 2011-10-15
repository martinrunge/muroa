/*
 * CAvahiThread.h
 *
 *  Created on: 11 Oct 2011
 *      Author: martin
 */

#ifndef CAVAHITHREAD_H_
#define CAVAHITHREAD_H_

#include <boost/asio.hpp>

class CAvahiThread  {
public:
	CAvahiThread( boost::asio::io_service& io_service );
	virtual ~CAvahiThread();

	void operator()() const;

private:

	boost::asio::io_service& m_io_service;
	int setNonblocking(int fd) const;
};

#endif /* CAVAHITHREAD_H_ */
