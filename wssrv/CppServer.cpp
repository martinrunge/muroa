/*
Copyright (c) 2002-2014 "Martin Runge"

server.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

Muroa is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "CppServer.h"

#include <log4cplus/loggingmacros.h>
#include <jsoncpp/json/json.h>

#include <CApp.h>
#include <Exceptions.h>
#include "CWSMsgHandler.h"

using namespace muroa;
using namespace std;



CppServer::CppServer(boost::asio::io_service& io_service, vector<string> clients, std::string session_name, int timeServerPort, int sessionID )
                  : CStreamServer(io_service, session_name, timeServerPort, sessionID, 2000),
                    m_io_service(io_service),
					m_decoder(0),
					m_selected_clients(clients)
{
	m_selected_clients = CApp::settings().getPersisentVal("msessiond", m_selected_clients);
}

CppServer::~CppServer() {
	stopStream();
}

// void CppServer::setWSSrv(muroa::WSSrv *wssrv) {
// 	m_ws_msg_handler = wssrv;
// }

/**
 * Add client to this session
 */
void CppServer::requestClientToJoin(std::string serviceName) {

    try {
        CStreamCtrlConnection* conn = getCtrlConnByName(serviceName );

        evRequestJoin* evj = new evRequestJoin();
        evj->m_session_name = m_session_name;
        // evj->m_mcast_addr =
        evj->m_timesrv_port = 12345;

        conn->onEvent(evj);
    }
    catch(CUnknownServiceNameException ex) {
		LOG4CPLUS_WARN(CApp::logger(), "CppServer::requestClientToJoin: no control connection to '" << serviceName << "' (" << ex.reason() << ")");
	}

}

/**
 * remove client from this session
 */
void CppServer::requestClientToLeave(std::string serviceName) {

	CStreamServer::requestLeave(serviceName);
	LOG4CPLUS_INFO(CApp::logger(), "CppServer::requestClientToLeave('" << serviceName << "')");

	// do not request client to join session automatically as soon as it appears any more
	removeClientFromSelected(serviceName);
}



void CppServer::playStream(const Json::Value& audio_src) {
	muroa::CStreamFmt new_sfmt;

    const string audio_src_name = audio_src["Name"].asString();
    LOG4CPLUS_INFO(CApp::logger(), "About to play audio source: '" << audio_src_name << "'");

    if(m_decoder != 0) {
        flush();
        delete m_decoder;
        close();
    }
    m_decoder = new CStreamDecoder(this);

    const string type = audio_src["type"].asString();

    if(type.compare("netstream") == 0) {
        const string url = audio_src["URL"].asString();
        LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: about to open url: " << url);
        new_sfmt = m_decoder->openUrl(url);

    } else if(type.compare("file") == 0) {
        const string filename = audio_src["filename"].asString();
        LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: about to open file: " << filename);
        new_sfmt = m_decoder->openFile(filename);

    } else if(type.compare("alsa") == 0) {
        const string device = audio_src["device"].asString();
        int channels = -1;
        int sample_rate = -1;

        if(audio_src.isMember("channels")) {
            channels = audio_src["channels"].asInt();
        }
        if(audio_src.isMember("sample_rate")) {
            sample_rate = audio_src["sample_rate"].asInt();
        }

        LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: about to open alsa src: " << device << ", sr: " << sample_rate << ", ch: " << channels);
        new_sfmt = m_decoder->openAlsa(device, sample_rate, channels );

    } else if(type.compare("pulse") == 0) {
        // LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: about to open url: " << url);
        // new_sfmt = m_decoder->openPulse(url);
    }

	LOG4CPLUS_DEBUG(CApp::logger(), "CppServer::playStream: url opened: channels: " << new_sfmt.getNumChannels() << " samplerate: " << new_sfmt.getSampleRate());

	if(new_sfmt.isValid()) {
		if(isOpen() == false) {
			open(new_sfmt);
		}

		muroa::CStreamFmt old_sfmt(getStreamFmt());
		if( new_sfmt != old_sfmt) {
			close();
			open(new_sfmt);
		}

		m_decoder->startDecodingThread();
	}
}


void CppServer::playFile(const std::string filename)
{
    LOG4CPLUS_INFO(CApp::logger(), "About to play: '" << filename << "'");
    muroa::CStreamFmt new_sfmt;

    if(m_decoder != 0) {
        flush();
        delete m_decoder;
        close();
    }
    m_decoder = new CStreamDecoder(this);

    new_sfmt = m_decoder->openFile(filename);

    if(new_sfmt.isValid()) {
        if(isOpen() == false) {
            open(new_sfmt);
        }

        muroa::CStreamFmt old_sfmt(getStreamFmt());
        if( new_sfmt != old_sfmt) {
            close();
            open(new_sfmt);
        }

        m_decoder->startDecodingThread();
    }
    else {
    	ostringstream ss;
    	ss << "file '" << filename << "' not found";
    	string tmpstr(ss.str());
        throw ExRessourceNotFound(tmpstr);
    }
}

void CppServer::playUrl(const std::string  url, int timeout_in_ms) {
    LOG4CPLUS_INFO(CApp::logger(), "About to play: '" << url << "'");
    muroa::CStreamFmt new_sfmt;

    if(m_decoder != 0) {
        flush();
        delete m_decoder;
        close();
    }
    m_decoder = new CStreamDecoder(this);

    new_sfmt = m_decoder->openUrl(url, timeout_in_ms);

    if(new_sfmt.isValid()) {
        if(isOpen() == false) {
            open(new_sfmt);
        }

        muroa::CStreamFmt old_sfmt(getStreamFmt());
        if( new_sfmt != old_sfmt) {
            close();
            open(new_sfmt);
        }

        m_decoder->startDecodingThread();
    }
}



void CppServer::stopStream() {
	LOG4CPLUS_INFO( CApp::logger(), "Stop stream" );
	flush();
	if(m_decoder != 0) {
		delete m_decoder;
		m_decoder = 0;
	}
	close();
}

void CppServer::onClientRejectedSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evJoinRejected* evt) {
	// let the base class do the book keeping
	CStreamServer::onClientRejectedSessionMember(conn, evt);

	m_ws_msg_handler->listClients();
    m_ws_msg_handler->informUser("Could not join session", evt->m_message + " \n'" + evt->m_owner_session + "'");
}

void CppServer::onClientBecameSessionMember(muroa::CStreamCtrlConnection* conn, const muroa::evSessionState* evt) {
	// let the base class do the book keeping
	CStreamServer::onClientBecameSessionMember(conn, evt);

	// will try request 'selected clients' to join session as soon as their client state is known
	addClientToSelected(conn->getServiceName());

	m_ws_msg_handler->listClients();
}

void CppServer::onClientLeftSession(muroa::CStreamCtrlConnection* conn, const muroa::evLeave* evt) {
	// let the base class do the book keeping
	CStreamServer::onClientLeftSession(conn, evt);

	m_ws_msg_handler->listClients();
}

void CppServer::onVolume(muroa::CStreamCtrlConnection* conn, const muroa::evVolume* evt) {
    // let the base class do the book keeping
    CStreamServer::onVolume(conn, evt);

    m_ws_msg_handler->listClients();
}


void CppServer::onError(muroa::CStreamCtrlConnection* conn, const evJoinRejected* evt) {

}

void CppServer::onClientState(muroa::CStreamCtrlConnection* conn, const muroa::evClientState* evt) {
	// let the base class do the book keeping
	CStreamServer::onClientState(conn, evt);
    m_ws_msg_handler->listClients();
	string serviceName = conn->getServiceName();

    if( isClientSelected(serviceName) && evt->m_member_of_session.compare("") == 0) {
        requestClientToJoin(serviceName);
	}
}

/** @brief called by decoding thread to report progress
 *
 * Event is then fed into the asio event
 * loop to hand it over to the thread context that started the decoding thread.
 *
 * @param posInSecs
 * @param durationInSecs
 */
void CppServer::reportProgress( int posInSecs, int durationInSecs) {
    m_io_service.post(std::bind( &CppServer::onProgress, this, posInSecs, durationInSecs ));
}

/** @brief called by decoding thread to indicate end of stream
 *
 * Event is then fed into the asio event
 * loop to hand it over to the thread context that started the decoding thread.
 *
 * @param posInSecs
 * @param durationInSecs
 */
void CppServer::reportEndOfStream()  {
    m_io_service.post(std::bind( &CppServer::onEndOfStream, this ));
}


void CppServer::onClientAppeared(ServDescPtr srvPtr) {
	try {
        CStreamServer::onClientAppeared(srvPtr);
        m_ws_msg_handler->listClients();
    }
    catch(const CConnectionFailedException& cfe) {
        ;
    }
}

void CppServer::onClientDisappeared(ServDescPtr srvPtr) {
	CStreamServer::onClientDisappeared(srvPtr);
	m_ws_msg_handler->listClients();
}


void CppServer::onProgress(int posInSecs, int durationInSecs) {
	m_ws_msg_handler->reportProgress(posInSecs, durationInSecs);
}

void CppServer::onEndOfStream() {
    m_decoder->close();
    m_ws_msg_handler->reportEndOfStream();
}


void CppServer::addClientToSelected(const string& serviceName) {

	vector<string>::iterator it = std::find(m_selected_clients.begin(), m_selected_clients.end(), serviceName);
	if ( it == m_selected_clients.end() ) {
		// serviceName is not yet m_selected_clients
		m_selected_clients.push_back(serviceName);
	}
	storeClientList();
}

void CppServer::removeClientFromSelected(const string& serviceName) {

	vector<string>::iterator it;
	do {
		it = std::find(m_selected_clients.begin(), m_selected_clients.end(), serviceName);
		if ( it != m_selected_clients.end() ) {
			// serviceName is not yet m_selected_clients
			m_selected_clients.erase(it);
			storeClientList();
		}
	} while(it != m_selected_clients.end());
}

bool CppServer::isClientSelected(const string &serviceName) {
    return std::find(m_selected_clients.begin(), m_selected_clients.end(), serviceName) != m_selected_clients.end();
}

void CppServer::storeClientList() {
	CApp::settings().setPersistentVal("msessiond", m_selected_clients);
}


