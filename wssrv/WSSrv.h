/*
 * WSSrv.h
 *
 *  Created on: 19.12.2015
 *      Author: martin
 */

#ifndef LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_WSSRV_H_
#define LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_WSSRV_H_

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <string>

class CRessourceHandler;
class CWSMsgHandler;

namespace muroa {

class WSSrv {
    typedef websocketpp::connection_hdl connection_hdl;
    typedef websocketpp::server<websocketpp::config::asio> server;
    typedef websocketpp::lib::lock_guard<websocketpp::lib::mutex> scoped_lock;

public:
	WSSrv(boost::asio::io_service* ptr, CRessourceHandler* resHandler, CWSMsgHandler* wsMsgHandler);
	virtual ~WSSrv();

    void run(std::string docroot, uint16_t port);
    void setTimer();
    void onTimer(websocketpp::lib::error_code const & ec);
    void onHttp(connection_hdl hdl);
    void onOpen(connection_hdl hdl);
    bool on_ping(connection_hdl hdl, std::string msg);
    void on_pong(connection_hdl hdl, std::string msg);
    void on_message(connection_hdl hdl, server::message_ptr msg);

    void onClose(connection_hdl hdl);

    void sendToAll(std::string message);
    void sendTo(connection_hdl hdl, std::string message);

private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    server m_endpoint;
    con_list m_connections;
    server::timer_ptr m_timer;

    CRessourceHandler* m_resHandler;
    CWSMsgHandler* m_wsMsgHandler;
    std::string m_docroot;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_EXAMPLES_WEBRADIO_WITH_HTML_GUI_WSSRV_H_ */


