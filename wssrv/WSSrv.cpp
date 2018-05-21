/*
 * WSSrv.cpp
 *
 *  Created on: 19.12.2015
 *      Author: martin
 */

#include "WSSrv.h"

#include "CRessourceHandler.h"
#include "CWSMsgHandler.h"

#include "MuroaExceptions.h"

#include <CApp.h>

#include <map>
#include <log4cplus/loggingmacros.h>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;
using namespace std;

namespace muroa {

WSSrv::WSSrv(boost::asio::io_service* ptr, CRessourceHandler* resHandler, CWSMsgHandler* wsMsgHandler)
         : m_resHandler(resHandler), m_wsMsgHandler(wsMsgHandler)
{
    // set up access channels to only log interesting things
    // m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    // m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
    // m_endpoint.set_access_channels(websocketpp::log::alevel::app);

    // Initialize the Asio transport policy
    m_endpoint.init_asio(ptr);
    m_endpoint.set_reuse_addr(true);


    // Bind the handlers we are using
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;

    using websocketpp::lib::bind;
    m_endpoint.set_open_handler    (bind(&WSSrv::onOpen,	this, _1));
    m_endpoint.set_close_handler   (bind(&WSSrv::onClose,	this, _1));
    m_endpoint.set_ping_handler    (bind(&WSSrv::on_ping,   this, _1, _2));
    m_endpoint.set_pong_handler    (bind(&WSSrv::on_pong,   this, _1, _2));
    m_endpoint.set_message_handler (bind(&WSSrv::on_message,this, _1, _2));
    m_endpoint.set_http_handler    (bind(&WSSrv::onHttp,	this, _1));
}

WSSrv::~WSSrv() {
}

void WSSrv::run(std::string docroot, uint16_t port) {
	LOG4CPLUS_INFO( CApp::logger(), "Running embedded webserver server on port "<< port <<" using docroot=" << docroot );

    std::size_t found = docroot.find_last_of("/");
    if(found != docroot.size()) {
    	docroot.push_back('/');
    }

    m_docroot = docroot;

    // listen on specified port
    m_endpoint.listen(port);

    // Start the server accept loop
    m_endpoint.start_accept();

    // Set the initial timer to start telemetry
    // setTimer();
}

void WSSrv::sendToAll(string message) {
    con_list::iterator it;
    for (it = m_ws_connections.begin(); it != m_ws_connections.end(); ++it) {
        m_endpoint.send(*it, message, websocketpp::frame::opcode::text);
    }
}

void WSSrv::sendTo(connection_hdl hdl, std::string message) {
	m_endpoint.send(hdl, message,  websocketpp::frame::opcode::text);
}

void WSSrv::setTimer() {
    m_timer = m_endpoint.set_timer(
        1000,
        websocketpp::lib::bind(
            &WSSrv::onTimer,
            this,
            websocketpp::lib::placeholders::_1
        )
    );
}

void WSSrv::onTimer(websocketpp::lib::error_code const & ec) {
    if (ec) {
        // there was an error, stop telemetry
        m_endpoint.get_alog().write(websocketpp::log::alevel::app,
                "Timer Error: "+ec.message());
        return;
    }

    std::stringstream val;
    val << "count is blah";

    // Broadcast count to all connections
    con_list::iterator it;
    for (it = m_ws_connections.begin(); it != m_ws_connections.end(); ++it) {
        m_endpoint.send(*it,val.str(),websocketpp::frame::opcode::text);
    }

    // set timer for next telemetry check
    setTimer();
}

bool WSSrv::on_ping(connection_hdl hdl, std::string msg) {
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, "on_ping: "+msg);
	return true;
}

void WSSrv::on_pong(connection_hdl hdl, std::string msg) {
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, "on_pong: "+msg);
}


void WSSrv::on_message(connection_hdl hdl, server::message_ptr msg) {
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, "on_message: "+msg->get_header() + " - " + msg->get_payload());
    m_wsMsgHandler->onMessage( hdl, msg->get_header(),  msg->get_payload() );
}

void WSSrv::onHttp(connection_hdl hdl) {
    m_http_connections.insert(hdl);
    
    // Upgrade our connection handle to a full connection_ptr
    server::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);

    std::ifstream file;
    string filename = con->get_uri()->get_resource();
    path p(filename);
    std::string response;


	if( filename.find("/REST") == 0 ) {
		LOG4CPLUS_DEBUG( CApp::logger(), "http request in REST API: " << filename );

        map<string, string> query_params;

        size_t qpos = filename.find('?');
        if(qpos != string::npos) {
        	// there i a query string
        	std::string querystr = filename.substr(qpos + 1);
        	filename = filename.substr(0, qpos);

        	size_t amppos = 0;
        	while(amppos != string::npos) {
        		size_t old_amppos = amppos;
        		string single_query_str;
        		amppos = querystr.find('&', old_amppos);

        		single_query_str = querystr.substr(old_amppos, amppos);

        		size_t equalpos = single_query_str.find('=');

        		if(equalpos != string::npos) {
					string key = single_query_str.substr(0, equalpos);
					string value = single_query_str.substr(equalpos + 1);

					query_params.insert(std::make_pair(key, value));
        		}

        	}

        }
        try {
        	m_resHandler->handleREST(filename, query_params);
        }
        catch(ExRpcError rpcex) {
    		std::stringstream ss;
    		ss << "<!doctype html><html><head>"
    		   << "<title>Error 400 (Bad request)</title><body>"
    		   << "<h1>Error 400</h1>"
    		   << "<p>" << rpcex.what() << "</p>"
    		   << "</body></head></html>";
    		con->set_body(ss.str());
    		con->set_status(websocketpp::http::status_code::ok);
    		return;
        }
        catch(ExRessourceNotFound& rnf) {
    		std::stringstream ss;
    		ss << "<!doctype html><html><head>"
    		   << "<title>Error 200 (Ok)</title><body>"
    		   << "<h1>Error 200</h1>"
    		   << "<p>" << rnf.getReason() << "</p>"
    		   << "</body></head></html>";
    		con->set_body(ss.str());
    		con->set_status(websocketpp::http::status_code::ok);
    		return;
        }

		std::stringstream ss;
		con->set_body(ss.str());
		con->set_status(websocketpp::http::status_code::ok);
		return;

	}
	else {
        if(p.compare("/") == 0 ) {
			p /= "index.html";
		}
        p= path(m_docroot)/p;
		LOG4CPLUS_INFO( CApp::logger(), "http request1 for: " << p.string() );

        bool regular_file = true;
        if(is_directory(p)) {
            regular_file = false;
        }
        else {
            file.open(p.c_str(), std::ios::in);   // std::ifstream::open will not complain if asked to open a directory
            if(!file.is_open()) {
                regular_file = false;
            }
        }

		if (regular_file == false) {
			// 404 error
			std::stringstream ss;

			ss << "<!doctype html><html><head>"
			   << "<title>Error 404 (Resource not found)</title><body>"
			   << "<h1>Error 404</h1>"
			   << "<p>The requested URL " << filename << " was not found on this server.</p>"
			   << "</body></head></html>";

			con->set_body(ss.str());
			con->set_status(websocketpp::http::status_code::not_found);
			return;
		}

		file.seekg(0, std::ios::end);
		response.reserve(file.tellg());
		file.seekg(0, std::ios::beg);
	}

    response.assign((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

    con->set_body(response);
    con->set_status(websocketpp::http::status_code::ok);
}

void WSSrv::onOpen(connection_hdl hdl) {
    m_ws_connections.insert(hdl);
}

void WSSrv::onClose(connection_hdl hdl) {
    m_ws_connections.erase(hdl);
}


} /* namespace muroa */
