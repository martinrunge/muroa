/*
 * CWSMsgHandler.cpp
 *
 *  Created on: 16.05.2016
 *      Author: martin
 */

#include "CWSMsgHandler.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <sstream>

#include "CppServer.h"

using namespace std;

CWSMsgHandler::CWSMsgHandler(CppServer* cpp_server, const Json::Value& stations) : m_StreamSrv(cpp_server), m_stations(stations) {
}

CWSMsgHandler::~CWSMsgHandler() {
	// TODO Auto-generated destructor stub
}

void CWSMsgHandler::onMessage(std::string header, std::string payload) {

	Json::Value root;   // starts as "null"; will contain the root value after parsing
	istringstream iss(payload);
	iss >> root;

	// Get the value of the member of root named 'event', return '<none>' if there is no such member.
	string evtype = root.get("event", "<none>" ).asString();

	if(evtype.compare("<none>") == 0) {
		// error
		return;
	}
	else if(evtype.compare("playctrl") == 0) {
		playctrl(root);
	}
	else if(evtype.compare("changeStation") == 0) {
		changeStation(root);
	}

}


void CWSMsgHandler::playctrl(const Json::Value& root) {
	m_StreamSrv->stopStream();
}


void CWSMsgHandler::changeStation(const Json::Value& root) {

	string stationID = root["data"]["stationID"].asString();
	string URL;

	// Iterate over the sequence elements.
	for ( int index = 0; index < m_stations.size(); ++index ) {
		if( stationID.compare( m_stations[index]["ID"].asString() ) == 0 ) {
			URL = m_stations[index]["URL"].asString();
		}
	}
	m_StreamSrv->playStream(URL);
}

