/*
 * CRessourceHandler.h
 *
 *  Created on: 20.12.2015
 *      Author: martin
 */

#ifndef LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CRESSOURCEHANDLER_H_
#define LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CRESSOURCEHANDLER_H_

#include <string>
#include <map>
#include <jsoncpp/json/json.h>
#include "CWSSrvApp.h"

class CppServer;


class CRessourceHandler {
public:
	CRessourceHandler(CppServer* cpp_server, CWSSrvApp* ws_srv_app);
	virtual ~CRessourceHandler();

	void handleREST(std::string filename, std::map<std::string, std::string> query_map);

private:
	CppServer* m_StreamSrv;
	CWSSrvApp* m_ws_srv_app;
};


#endif /* LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CRESSOURCEHANDLER_H_ */
