//
// Created by martin on 22.01.17.
//

#include "CHttpRequest.h"

const std::string CHttpRequest::method_str[] = {"OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT", "PATCH"};


CHttpRequest::CHttpRequest(const std::string& server, const std::string& path, enum method method): m_server(server),
                                                                                      m_path(path),
                                                                                      m_method ( method )
{

    m_headers["Host"] = server;
    m_headers["Accept"] = "*/*";
    m_headers["Connection"] = "close";
}

CHttpRequest::~CHttpRequest() {

}


void CHttpRequest::getRawRequest(boost::asio::streambuf& buf) {

    std::ostream buf_stream(&buf);

    buf_stream << method_str[m_method] << " " << m_path << " HTTP/1.0\r\n";
    for(auto const& header_field : m_headers) {
        buf_stream << header_field.first << ": " << header_field.second << "\r\n";
    }
    buf_stream << "\r\n";
}
