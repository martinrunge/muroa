/*
 * CRessourceHandler.cpp
 *
 *  Created on: 20.12.2015
 *      Author: martin
 */

#include "CRessourceHandler.h"

#include "CppServer.h"
#include <fstream>

using namespace std;

CRessourceHandler::CRessourceHandler(CppServer* cpp_server, CWSSrvApp* ws_srv_app) : m_StreamSrv(cpp_server), m_ws_srv_app(ws_srv_app) {
}

CRessourceHandler::~CRessourceHandler() {

}

void CRessourceHandler::handleREST(std::string filename, std::map<string,string> query_map) {
	if(filename.compare("/REST/station.json") == 0) {
		map<string, string>::iterator it;
		it = query_map.find("station");
		if(it == query_map.end()) {
			// station not found -> repost error
		}
		string stationID = it->second;
		string URL;

		const Json::Value net_streams = m_ws_srv_app->getNetStreams();
		// Iterate over the sequence elements.
		for ( int index = 0; index < net_streams.size(); ++index ) {
			if( stationID.compare( net_streams[index]["ID"].asString() ) == 0 ) {
				URL = net_streams[index]["URL"].asString();
			}
		}

		m_StreamSrv->playStream(URL);

	}
	else if(filename.compare("/REST/play.json") == 0) {
		map<string, string>::iterator it;
		it = query_map.find("file");
		if(it == query_map.end()) {
			// station not found -> repost error
		}
		string filename = it->second;


		m_StreamSrv->playFile(filename);
	}
	else if(filename.compare("/REST/stop.json") == 0) {
		m_StreamSrv->stopStream();
	}
    else if(filename.compare("/REST/status.json") == 0) {
        // m_StreamSrv->getStatus();
    }
}


