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
#include "WSSrv.h"

using namespace std;
using namespace muroa;

CWSMsgHandler::CWSMsgHandler(CppServer* cpp_server, const Json::Value& stations) : m_StreamSrv(cpp_server), m_stations(stations) {
}

CWSMsgHandler::~CWSMsgHandler() {
	// TODO Auto-generated destructor stub
}

void CWSMsgHandler::onMessage(websocketpp::connection_hdl hdl, std::string header, std::string payload) {

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
	else if(evtype.compare("getCurrentClientList") == 0) {
		onListClients(hdl, root);
	}
	else if(evtype.compare("activateClient") == 0) {
		onActivateClient(hdl, root);
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

void CWSMsgHandler::listClients() {

	vector<CRenderClientDesc> rcs = m_StreamSrv->getRenderClients();
	Json::Value data(Json::arrayValue);

	std::vector<CRenderClientDesc>::const_iterator it;
	for(it = rcs.begin(); it != rcs.end(); it++) {
		Json::Value elem;
		elem["name"] = it->srvPtr->getServiceName();
		elem["hostname"] = it->srvPtr->getHostName();
		elem["ownersession"] = it->getMemberOfSession();
		elem["member"] = it->isMember();
		elem["online"] = it->isOnline();
		data.append(elem);
	}

	Json::Value ret;
	ret["event"] = "muroad";
	ret["data"] = data;

	ostringstream oss;
	oss << ret;

	string json_msg = oss.str();
	m_ws_srv->sendToAll(json_msg);

}

void CWSMsgHandler::onListClients(connection_hdl hdl, Json::Value root) {

	vector<CRenderClientDesc> rcs = m_StreamSrv->getRenderClients();
	Json::Value data(Json::arrayValue);

	std::vector<CRenderClientDesc>::const_iterator it;
	for(it = rcs.begin(); it != rcs.end(); it++) {
		Json::Value elem;
		elem["name"] = it->srvPtr->getServiceName();
		elem["hostname"] = it->srvPtr->getHostName();
		elem["ownersession"] = it->getMemberOfSession();
		elem["member"] = it->isMember();
		elem["online"] = it->isOnline();
		data.append(elem);
	}

	Json::Value ret;
	ret["event"] = "muroad";
	ret["data"] = data;

	ostringstream oss;
	oss << ret;

	string json_msg = oss.str();
	m_ws_srv->sendTo(hdl, json_msg);

}

void CWSMsgHandler::informUser(std::string caption, std::string message) {
    Json::Value data(Json::objectValue);

    data["caption"] = caption;
    data["message"] = message;

    Json::Value ret;
    ret["event"] = "inform";
    ret["data"] = data;

    ostringstream oss;
    oss << ret;

    string json_msg = oss.str();
    m_ws_srv->sendToAll(json_msg);
}


/**
 * activate = true:
 *   if there is already an open control connection to that client, use it to request
 *   it to join the session, otherwiese open a new control connection to that client first.
 *
 * activate = false:
 *   if client is session member, request leave, otherwise do nothing
 */
void CWSMsgHandler::onActivateClient(connection_hdl hdl, Json::Value root) {
	string serviceName = root["data"]["serviceName"].asString();
	bool activate = root["data"]["activate"].asBool();
	if(activate == true) {
		m_StreamSrv->requestClientToJoin( serviceName );
	}
	else {
		m_StreamSrv->requestClientToLeave( serviceName );
	}
}

