/*
 * CWSMsgHandler.h
 *
 *  Created on: 16.05.2016
 *      Author: martin
 */

#ifndef LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CWSMSGHANDLER_H_
#define LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CWSMSGHANDLER_H_

#include <jsoncpp/json/json.h>

class CppServer;

class CWSMsgHandler {
public:
	CWSMsgHandler(CppServer* cpp_server, const Json::Value& stations);
	virtual ~CWSMsgHandler();

	void onMessage(std::string header, std::string payload);

private:
	void playctrl(const Json::Value& root);
	void changeStation(const Json::Value& root);

	CppServer* m_StreamSrv;
	const Json::Value& m_stations;

};

#endif /* LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CWSMSGHANDLER_H_ */
