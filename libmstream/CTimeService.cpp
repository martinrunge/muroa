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

namespace muroa {

using namespace boost;
using namespace boost::asio::ip;

CTimeService::CTimeService(boost::asio::io_service& io_serv,
		                   bool server_role,
		                   address ip_address,
		                   int portNr,
		                   udp protocol)
                           :
                           m_server_role(server_role),
		                   m_ip_address(ip_address),
		                   m_port_nr(portNr),
		                   m_socket(io_serv),
		                   m_timer(io_serv)
{
	try {
		m_socket.open(protocol);
		if(m_server_role) {

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

			}
			m_port_nr = portNr;
			start_server();
		}
		else {
			m_remote_endpoint = udp::endpoint(m_ip_address, m_port_nr);
			send_request(system::error_code());
		}
	}
	catch(boost::system::system_error err) {
		// could not open socket;
	}

}

CTimeService::~CTimeService()
{
	m_timer.cancel();
}

void CTimeService::start_server() {
	m_socket.async_receive_from(asio::buffer(m_buffer), m_remote_endpoint,
                                boost::bind(&CTimeService::handle_receive, this,
                                            asio::placeholders::error,
                                            asio::placeholders::bytes_transferred) );
}

void CTimeService::start_timer() {
	m_timer.async_wait( boost::bind(&CTimeService::send_request, shared_from_this(), asio::placeholders::error) );
}

void CTimeService::send_request(const boost::system::error_code& ec ) {
	if(ec != boost::asio::error::operation_aborted) {
		CTime send_time;
		CTimeSyncPkt ts_pkt;
		ts_pkt.setT1(send_time);

		buffer_t buf = ts_pkt.serialize();

		boost::system::error_code send_ec;
		size_t bytes_sent = m_socket.send_to(asio::buffer(buf.buffer, buf.size), m_remote_endpoint, 0, send_ec);

		start_timer();
	}
}

void CTimeService::handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred) {

	if(!ec || ec == boost::asio::error::message_size) {
		CTime recv_time;
		CTimeSyncPkt ts_pkt(m_buffer.data(), bytes_transferred);
		if(m_server_role) {
			ts_pkt.setT1Tick(recv_time);

			CTime send_time;
			ts_pkt.setT2(send_time);
			buffer_t buf = ts_pkt.serialize();

			boost::system::error_code send_ec;
			size_t bytes_sent = m_socket.send_to(asio::buffer(buf.buffer, buf.size), m_remote_endpoint, 0, send_ec);

			start_server();
		}
		else {
			ts_pkt.setT2Tick(recv_time);
			calcTimeDiff(&ts_pkt);
			start_timer();
		}
	}
}

void CTimeService::createResponse(size_t bytes_transferred) {
	boost::system::error_code ec;


}

void CTimeService::calcTimeDiff(CTimeSyncPkt *ts_pkt) {

}

} /* namespace muroa */
