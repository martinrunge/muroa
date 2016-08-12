/*
 * CHttpClient.h
 *
 *  Created on: 13.12.2015
 *      Author: martin
 */
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

#ifndef CHTTPCLIENT_H_
#define CHTTPCLIENT_H_

namespace muroa {

class CHttpClient {
public:
	CHttpClient(boost::asio::io_service& io_service, const std::string& server, const std::string& path);
	virtual ~CHttpClient();

private:
	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err);
	void handle_write_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_content(const boost::system::error_code& err);

	tcp::resolver m_resolver;
	tcp::socket m_socket;
	boost::asio::streambuf m_request;
	boost::asio::streambuf m_response;
};

} /* namespace muroa */


#endif /* CHTTPCLIENT_H_ */
