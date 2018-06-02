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

CWSMsgHandler::CWSMsgHandler(CppServer* cpp_server, CWSSrvApp* ws_srv_app) : m_StreamSrv(cpp_server), m_ws_srv_app(ws_srv_app) {
}

CWSMsgHandler::~CWSMsgHandler() {
	// TODO Auto-generated destructor stub
}

void CWSMsgHandler::onMessage(websocketpp::connection_hdl hdl, std::string header, std::string payload) {

	Json::Value root;   // starts as "null"; will contain the root value after parsing
	istringstream iss(payload);
	try {
		iss >> root;

		// Get the value of the member of root named 'event', return '<none>' if there is no such member.
		string evtype = root.get("event", "<none>").asString();

		if (evtype.compare("<none>") == 0) {
            string jsonrpcver = root.get("jsonrpc", "<none>").asString();
            if( jsonrpcver.compare("2.0") == 0) {
                // JSON RPC 2.0
                int jsonrpcid = root.get("id", "0").asInt();
                string method = root.get("method", "<none>").asString();
                Json::Value params = root.get("params", Json::Value());

                if(method.compare("playctrl") == 0) {
                    playctrl(hdl, params, jsonrpcid);
                }

            }
			// error
			return;
		}
		else if (evtype.compare("playctrl") == 0) {
			playctrl(hdl, root, 0);
		}
		else if (evtype.compare("changeStation") == 0) {
			changeStation(root);
		}
		else if (evtype.compare("getCurrentClientList") == 0) {
			onListClients(hdl, root);
		}
		else if (evtype.compare("adjVol") == 0) {
			onAdjVol(hdl, root);
		}
		else if (evtype.compare("activateClient") == 0) {
			onActivateClient(hdl, root);
		}
	}
	catch(std::exception ex) {
		cerr << ex.what();
	}
}


void CWSMsgHandler::playctrl(websocketpp::connection_hdl hdl, const Json::Value& params, int jsonrpcid) {
	string filename = params.get("file", "<none>").asString();
	if(filename.compare("<none>") == 0) {
		string url = params.get("url", "<none>").asString();
		if(url.compare("<none>") == 0) {
			reportError(hdl, "error in jsonrpc message: neither file nor url given", jsonrpcid);
		}
	}
	try {
		m_StreamSrv->playFile(filename);

	}
	catch( ExRessourceNotFound ex) {
		reportError(hdl, ex.getReason(), jsonrpcid);
	}
}


void CWSMsgHandler::changeStation(const Json::Value& root) {

	string stationID = root["data"]["stationID"].asString();
	string type;

    const Json::Value net_streams = m_ws_srv_app->getNetStreams();
	// Iterate over the sequence elements.
	for ( int index = 0; index < net_streams.size(); ++index ) {
		if( stationID.compare( net_streams[index]["ID"].asString() ) == 0 ) {
            const Json::Value& audio_src = net_streams[index];
            m_StreamSrv->playStream(audio_src);
		}
	}
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
        elem["volume"] = it->getVolume();
		elem["online"] = it->isOnline();
		elem["lasterrmsg"] = it->getLastErrorMsg();
		elem["lastsyncerr"] = it->getLastErrorMsg();
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
        elem["volume"] = it->getVolume();
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

void CWSMsgHandler::onAdjVol(CWSMsgHandler::connection_hdl hdl, Json::Value root) {
    string serviceName = root["data"]["serviceName"].asString();
    int volume = root["data"]["volume"].asInt();
    m_StreamSrv->setVolume(serviceName, volume);

}


void CWSMsgHandler::reportProgress(int posInSecs, int durationInSecs) {
	Json::Value params;
	params["pos_in_secs"] = posInSecs;
	params["duration_in_secs"] = durationInSecs;

	Json::Value resp;
	resp["jsonrpc"] = "2.0";
	resp["method"] = "progress";
	resp["params"] = params;

	ostringstream oss;
	oss << resp;
	string json_msg = oss.str();
	m_ws_srv->sendToAll(json_msg);
}

void CWSMsgHandler::reportEndOfStream() {
	Json::Value resp;
	resp["jsonrpc"] = "2.0";
	resp["method"] = "end_of_stream";
	resp["params"] = Json::nullValue;

	ostringstream oss;
	oss << resp;
	string json_msg = oss.str();
	m_ws_srv->sendToAll(json_msg);
}

void CWSMsgHandler::reportError(CWSMsgHandler::connection_hdl hdl, std::string errormsg, int jsonrpcid)
{
	Json::Value result;
	result["success"] = false;
	result["errormsg"] = errormsg;

	Json::Value resp;
	resp["jsonrpc"] = "2.0";
	resp["result"] = result;
	resp["id"] = jsonrpcid;

	ostringstream oss;
	oss << resp;
	string json_msg = oss.str();
	m_ws_srv->sendTo(hdl, json_msg);
}


void CWSMsgHandler::reportSessionError(std::string client_name, std::string errormsg, int64_t clock_offset) {   // , int64_t last_sync_error) {
	Json::Value params;
	params["client"] = client_name;
	params["errormsg"] = errormsg;
	params["clockoffset"] = Json::Value::Int64(clock_offset);
    //params["last_sync_error"] = last_sync_error;

    Json::Value notification;
    notification["jsonrpc"] = "2.0";
    notification["method"] = "session_error";
    notification["params"] = params;

	ostringstream oss;
	oss << notification;
	string json_msg = oss.str();
	m_ws_srv->sendToAll(json_msg);
}

