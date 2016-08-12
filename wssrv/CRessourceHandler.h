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
class CppServer;


class CRessourceHandler {
public:
	CRessourceHandler(CppServer* cpp_server, const Json::Value& stations);
	virtual ~CRessourceHandler();

	void handleREST(std::string filename, std::map<std::string, std::string> query_map);

private:
	CppServer* m_StreamSrv;
	const Json::Value& m_stations;
};


#endif /* LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_CRESSOURCEHANDLER_H_ */
