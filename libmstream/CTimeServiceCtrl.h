/*
 Copyright (c) 2002-2014 "Martin Runge"

 CTimeServiceCtrl.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef LIBMSTREAM_CTIMESERVICECTRL_H_
#define LIBMSTREAM_CTIMESERVICECTRL_H_

#include <thread>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable


// 	#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/loggingmacros.h>

namespace muroa {

class CTimeServiceCtrl {
public:
	CTimeServiceCtrl();
	virtual ~CTimeServiceCtrl();

	void startServer(int portNr, boost::asio::ip::udp protocol = boost::asio::ip::udp::v4());
	void startClient(boost::asio::ip::udp::endpoint timesrv_endpoint);
	void stop();

	int getCurrentServerPort();

private:
	void server_thread_func(int portNr, boost::asio::ip::udp protocol);
	void client_thread_func(boost::asio::ip::udp::endpoint timesrv_endpoint);

	std::thread *m_thread;

	std::mutex m_mutex;
	std::condition_variable m_cond_var;
	bool m_server_role;
	int m_used_port;

	log4cplus::Logger m_logger;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CTIMESERVICECTRL_H_ */
