/***************************************************************************
 *
 *   CSignalHandler.h
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

#ifndef CSIGNALHANDLER_H_
#define CSIGNALHANDLER_H_

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <signal.h>

class CSignalHandler : public boost::enable_shared_from_this<CSignalHandler> {
public:
	virtual ~CSignalHandler();
	typedef boost::shared_ptr<CSignalHandler> pointer;

	static pointer create(boost::asio::io_service& io_service) {
		return pointer(new CSignalHandler(io_service));
	}

	void start();

private:
	CSignalHandler(boost::asio::io_service& io_service);
	void installSigHandlers();

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	boost::asio::local::stream_protocol::socket m_socket;
	// boost::asio::local::stream_protocol::socket m_handler_socket;

	struct sigaction  m_old_sigaction;

    /// Buffer for incoming data.
	boost::array<char, 8192> m_buffer;
};

#endif /* CSIGNALHANDLER_H_ */
