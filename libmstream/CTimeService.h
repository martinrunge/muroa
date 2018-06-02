/*
 Copyright (c) 2002-2014 "Martin Runge"

 CTimeService.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CTIMESERVICE_H_
#define LIBMSTREAM_CTIMESERVICE_H_

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace muroa {

class CTimeSyncPkt;
class CTimeServiceCtrl;

class CTimeService : public boost::enable_shared_from_this<CTimeService>{
public:
	CTimeService(boost::asio::io_service& io_service, CTimeServiceCtrl* const tsctrl, int portNr, boost::asio::ip::udp protocol = boost::asio::ip::udp::v4());
	CTimeService(boost::asio::io_service& io_service, CTimeServiceCtrl* const tsctrl, boost::asio::ip::udp::endpoint timesrv_endpoint);
	virtual ~CTimeService();

	int getUsedPort() { return m_port_nr; };

	void stop();

private:
	boost::asio::ip::address m_ip_address;
	int m_port_nr;
	bool m_server_role;
	CTimeServiceCtrl* const m_ts_ctrl;

	boost::asio::ip::udp::socket m_socket;
	boost::asio::ip::udp::endpoint m_remote_endpoint;

	boost::asio::deadline_timer m_timer;

	bool m_awaiting_response;
	bool m_shutting_down;

	void start_server();
	void start_timer();
	void send_request(const boost::system::error_code& ec);

	void handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred);

	void createResponse(std::size_t bytes_transferred);
	void calcTimeDiff(CTimeSyncPkt *ts_pkt);

	  /// Buffer for incoming data.
	boost::array<char, 8192> m_buffer;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CTIMESERVICE_H_ */
