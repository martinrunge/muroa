/*
 * CWSMsgHandler.h
 *
 *  Created on: 16.05.2016
 *      Author: martin
 */

#ifndef LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CWSMSGHANDLER_H_
#define LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CWSMSGHANDLER_H_

#include <jsoncpp/json/json.h>
#include <websocketpp/server.hpp>
#include "CWSSrvApp.h"


class CppServer;
namespace muroa
{
  class WSSrv;
}

class CWSMsgHandler {
	typedef websocketpp::connection_hdl connection_hdl;

public:
	CWSMsgHandler(CppServer* cpp_server, CWSSrvApp* ws_srv_app);
	virtual ~CWSMsgHandler();

	void setWSSrv(muroa::WSSrv *ws_srv) { m_ws_srv= ws_srv; };

	void onMessage(connection_hdl hdl, std::string header, std::string payload);
	void listClients();
	void informUser(std::string caption, std::string message);

protected:
	void onListClients(connection_hdl hdl, Json::Value root);
	void onActivateClient(connection_hdl hdl, Json::Value root);

private:
	void playctrl(const Json::Value& root);
	void changeStation(const Json::Value& root);

	CppServer* m_StreamSrv;
	muroa::WSSrv* m_ws_srv;

	CWSSrvApp* m_ws_srv_app;

	void onAdjVol(connection_hdl hdl, Json::Value root);
};

#endif /* LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CWSMSGHANDLER_H_ */
