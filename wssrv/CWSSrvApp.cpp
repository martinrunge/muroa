//
// Created by martin on 12.02.17.
//

#include <jsoncpp/json/json.h>
#include <fstream>
#include <CApp.h>
#include "CWSSrvApp.h"

using namespace std;
using namespace muroa;

CWSSrvApp::CWSSrvApp(muroa::CApp* app) : m_app(app) {
    m_input_devices = m_app->settings().getMediaSources();

}

void CWSSrvApp::setNetStreamConfig(boost::filesystem::path path) {

    Json::Reader reader;
    ifstream ifs;
    ifs.open(path.string());
    reader.parse(ifs, m_net_streams);
    ifs.close();

}

const IMediaSrcDesc *CWSSrvApp::getInputDeviceDesc(std::string id) {
    return m_input_devices[id];
}



