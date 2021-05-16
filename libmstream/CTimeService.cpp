/*
 Copyright (c) 2002-2014 "Martin Runge"

 CTimeService.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CTimeService.h"
#include "CTime.h"
#include "CTimeSyncPkt.h"
#include "CTimeServiceCtrl.h"
#include <iostream>

namespace muroa {

using namespace boost;
using namespace boost::asio::ip;

// c-tor for server mode
CTimeService::CTimeService(boost::asio::io_context& io_context, CTimeServiceCtrl* const tsctrl, int portNr, udp protocol)
                           :
                           m_server_role(true),
						   m_ts_ctrl(tsctrl),
		                   m_port_nr(portNr),
		                   m_socket(io_context),
		                   m_timer(io_context),
						   m_awaiting_response(false),
						   m_shutting_down(false),
						   m_io_context(io_context)
{
	try {
		m_socket.open(protocol);
		bool found = false;
		while( !found ) {
			udp::endpoint endpoint = udp::endpoint(protocol, portNr);
			boost::system::error_code ec;
			m_socket.bind(endpoint, ec);
			int val = ec.value();
			if( val == EADDRINUSE ) {
				portNr++;
			}
			else {
				found = true;
			}
			m_port_nr = portNr;
			start_server();
		}
	}
	catch(boost::system::system_error err) {
		// could not open socket;
	}

}

// c-tor for client mode
CTimeService::CTimeService(boost::asio::io_context& io_context, CTimeServiceCtrl* const tsctrl, boost::asio::ip::udp::endpoint timesrv_endpoint)
                           :
                           m_server_role(false),
						   m_ts_ctrl(tsctrl),
		                   m_socket(io_context),
		                   m_timer(io_context),
						   m_awaiting_response(false),
						   m_shutting_down(false),
						   m_io_context(io_context)
{
	try {

		m_socket.open(timesrv_endpoint.protocol());

		m_remote_endpoint = timesrv_endpoint;
		send_request(system::error_code());
	}
	catch(boost::system::system_error err) {
		// could not open socket;
	}

}

CTimeService::~CTimeService()
{
	m_timer.cancel();
	m_socket.close();
}

void CTimeService::start_server() {
	m_socket.async_receive_from(asio::buffer(m_buffer), m_remote_endpoint,
                                boost::bind(&CTimeService::handle_receive, this,
                                            asio::placeholders::error,
                                            asio::placeholders::bytes_transferred) );
}

void CTimeService::start_timer() {
	m_timer.expires_from_now(boost::posix_time::seconds(5));
	m_timer.async_wait( boost::bind(&CTimeService::send_request, this, asio::placeholders::error) );
}

void CTimeService::send_request(const boost::system::error_code& ec ) {
	if(ec != boost::asio::error::operation_aborted) {
		if(m_awaiting_response) {
			// error
		}
		CTimeSyncPkt ts_pkt;
		ts_pkt.setT1(CTime::now());

		buffer_t buf = ts_pkt.serialize();

		boost::system::error_code send_ec;
		size_t bytes_sent = m_socket.send_to(asio::buffer(buf.buffer, buf.size), m_remote_endpoint, 0, send_ec);

		m_awaiting_response = true;
		m_socket.async_receive_from(asio::buffer(m_buffer), m_remote_endpoint, boost::bind(&CTimeService::handle_receive, this,
	                                            asio::placeholders::error,
	                                            asio::placeholders::bytes_transferred) );
	}
}

void CTimeService::handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred) {

	if(!ec || ec == boost::asio::error::message_size) {
		CTime recv_time(CTime::now());
		CTimeSyncPkt ts_pkt(m_buffer.data(), bytes_transferred);
		if(m_server_role) {
			ts_pkt.setT2(recv_time);

			ts_pkt.setT3(CTime::now());
			buffer_t buf = ts_pkt.serialize();

			boost::system::error_code send_ec;
			size_t bytes_sent = m_socket.send_to(asio::buffer(buf.buffer, buf.size), m_remote_endpoint, 0, send_ec);

			start_server();
		}
		else {
			if(m_awaiting_response == false) {
				// error
			}
			ts_pkt.setT4(recv_time);
			CDuration theta = ts_pkt.getClockOffset();
			std::cerr << " clock offset [s]: " << std::fixed << theta.sec() << std::endl;
			m_ts_ctrl->onClockOffset(theta);
			m_awaiting_response = false;
			start_timer();
		}
	}
}

void CTimeService::createResponse(size_t bytes_transferred) {
	boost::system::error_code ec;


}

void CTimeService::stop() {
	m_shutting_down = true;
	m_io_context.stop();
	m_socket.cancel();
	m_timer.cancel();
}

} /* namespace muroa */
