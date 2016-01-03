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

CRessourceHandler::CRessourceHandler(CppServer* cpp_server, string stations_file) : m_StreamSrv(cpp_server) {
	Json::Reader reader;

	ifstream ifs;
	ifs.open(stations_file);

	reader.parse(ifs, m_stations);

	ifs.close();
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

		// Iterate over the sequence elements.
		for ( int index = 0; index < m_stations.size(); ++index ) {
			if( stationID.compare( m_stations[index]["ID"].asString() ) == 0 ) {
				URL = m_stations[index]["URL"].asString();
			}
		}

		m_StreamSrv->playStream(URL);

	}
	else if(filename.compare("/REST/stop.json") == 0) {
		m_StreamSrv->stopStream();
	}

}


