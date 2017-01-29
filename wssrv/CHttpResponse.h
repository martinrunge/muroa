//
// Created by martin on 22.01.17.
//

#ifndef MUROA_CHTTPRESPONSE_H
#define MUROA_CHTTPRESPONSE_H


#include <string>
#include <map>

class CHttpResponse {

public:
    void  setStatus(unsigned int code, const std::string& message) {
        m_code = code;
        m_message = message;
    };

private:
    unsigned int m_code;
    std::string m_message;

    std::map<std::string, std::string> m_headers;

};


#endif //MUROA_CHTTPRESPONSE_H
