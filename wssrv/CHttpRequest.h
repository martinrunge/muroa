//
// Created by martin on 22.01.17.
//

#ifndef MUROA_CHTTPREQUEST_H
#define MUROA_CHTTPREQUEST_H

#include <string>
#include <map>
#include <boost/asio.hpp>

class CHttpRequest {
    enum method { OPTIONS = 0, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT, PATCH };
    static const std::string method_str[];
public:
    CHttpRequest(const std::string& server, const std::string& path, enum method method = GET);
    ~CHttpRequest();

    void getRawRequest(boost::asio::streambuf& buf);

private:
    std::map<std::string, std::string> m_headers;

    std::string m_server;
    std::string m_path;
    enum method m_method;
};


#endif //MUROA_CHTTPREQUEST_H
