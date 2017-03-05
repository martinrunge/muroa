//
// Created by martin on 12.02.17.
//

#ifndef MUROA_CWSSRVAPP_H
#define MUROA_CWSSRVAPP_H


#include <iostream>
#include <boost/filesystem/path.hpp>
#include <CApp.h>
#include "IMediaSrcDesc.h"

class CWSSrvApp {

public:
    CWSSrvApp(muroa::CApp* app);
    void setNetStreamConfig(boost::filesystem::path path);

    const Json::Value& getNetStreams() {
        return m_net_streams;
    }

    const muroa::IMediaSrcDesc* getInputDeviceDesc(std::string id);

private:
    muroa::CApp* m_app;
    Json::Value m_net_streams;

    std::map<std::string, muroa::IMediaSrcDesc*> m_input_devices;
    std::map<std::string, muroa::IMediaSrcDesc*> m_net_stream_vec;

};


#endif //MUROA_CWSSRVAPP_H
