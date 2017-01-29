/*
 * CHttpClient.cpp
 *
 *  Created on: 13.12.2015
 *      Author: martin
 */
//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "CHttpClient.h"

namespace muroa {
using boost::asio::ip::tcp;

CHttpClient::CHttpClient(boost::asio::io_service& io_service, const std::string& server, const std::string& path)
            	    : m_resolver(io_service),
					  m_socket(io_service),
                      m_request(server, path)
{

    CHttpRequest req(server, path);
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
//    std::ostream request_stream(&m_request);
//    request_stream << "GET " << path << " HTTP/1.0\r\n";
//    request_stream << "Host: " << server << "\r\n";
//    request_stream << "Accept: */*\r\n";
//    request_stream << "Connection: close\r\n\r\n";


    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, "http");
    m_resolver.async_resolve(query,
        boost::bind(&CHttpClient::handle_resolve, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator));
}

CHttpClient::~CHttpClient() {
	// TODO Auto-generated destructor stub
}




void CHttpClient::handle_resolve(const boost::system::error_code& err,
		tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		// Attempt a connection to each endpoint in the list until we
		// successfully establish a connection.
		boost::asio::async_connect(m_socket, endpoint_iterator,
				boost::bind(&CHttpClient::handle_connect, this,
						boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void CHttpClient::handle_connect(const boost::system::error_code& err)
{
	if (!err)
	{
        boost::asio::streambuf request_buf;
        m_request.getRawRequest(request_buf);
        // The connection was successful. Send the request.
		boost::asio::async_write(m_socket, request_buf,
				boost::bind(&CHttpClient::handle_write_request, this,
						boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void CHttpClient::handle_write_request(const boost::system::error_code& err)
{
	if (!err)
	{
		// Read the response status line. The m_response streambuf will
		// automatically grow to accommodate the entire line. The growth may be
		// limited by passing a maximum size to the streambuf constructor.
		boost::asio::async_read_until(m_socket, m_response_buf, "\r\n",
				boost::bind(&CHttpClient::handle_read_status_line, this,
						boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void CHttpClient::handle_read_status_line(const boost::system::error_code& err)
{
	if (!err)
	{
		// Check that response is OK.
		std::istream response_stream(&m_response_buf);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return;
		}
        m_response.setStatus(status_code, status_message);

		// Read the response headers, which are terminated by a blank line.
		boost::asio::async_read_until(m_socket, m_response_buf, "\r\n\r\n",
				boost::bind(&CHttpClient::handle_read_headers, this,
						boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

void CHttpClient::handle_read_headers(const boost::system::error_code& err)
{
	if (!err)
	{
		// Process the response headers.
		std::istream response_stream(&m_response_buf);
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		if (m_response_buf.size() > 0)
			std::cout << &m_response;

		// Start reading remaining data until EOF.
		boost::asio::async_read(m_socket, m_response_buf,
				boost::asio::transfer_at_least(1),
				boost::bind(&CHttpClient::handle_read_content, this,
						boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

void CHttpClient::handle_read_content(const boost::system::error_code& err)
{
	if (!err)
	{
		// Write all of the data that has been read so far.
		std::cout << &m_response;

		// Continue reading remaining data until EOF.
		boost::asio::async_read(m_socket, m_response_buf,
				boost::asio::transfer_at_least(1),
				boost::bind(&CHttpClient::handle_read_content, this,
						boost::asio::placeholders::error));
	}
	else if (err != boost::asio::error::eof)
	{
		std::cout << "Error: " << err << "\n";
	}
}


} /* namespace muroa */

