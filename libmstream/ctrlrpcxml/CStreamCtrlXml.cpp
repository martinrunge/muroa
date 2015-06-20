 /**************************************************************************
 *                                                                         *
 *   CStreamCtrlXml.cpp                                                    *
 *                                                                         *
 *   This file is part of libmstream                                       *
 *   Copyright (C) 2014 by Martin Runge <martin.runge@web.de>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <cmds/CmdStream.h>
#include "CStreamCtrlXml.h"
#include "StreamCtrlXMLCmds.h"

#include <sstream>
#include <iostream>
#include <typeinfo>

using namespace std;
using namespace muroa;
using namespace boost::asio;

namespace muroa {

CStreamCtrlXml::CStreamCtrlXml() throw (ExRpcError) {
	m_parser = XML_ParserCreate(NULL);
	if (!m_parser) {
		// fprintf(stderr, "Couldn't allocate memory for parser\n");
		ExRpcError ex("no parser");
		throw ex;
	}

	XML_SetUserData(m_parser, (void*) this);
	XML_SetElementHandler(m_parser, CStreamCtrlXml::startTagHandler,	CStreamCtrlXml::endTagHandler);
	XML_SetCharacterDataHandler(m_parser, CStreamCtrlXml::characterHandler);

	type_serializers[std::type_index(typeid( evClientState     ))] = &CStreamCtrlXml::sendEvClientState;
	type_serializers[std::type_index(typeid( evRequestJoin     ))] = &CStreamCtrlXml::sendEvRequestJoin;
	type_serializers[std::type_index(typeid( evJoinAccepted    ))] = &CStreamCtrlXml::sendEvJoinAccepted;
	type_serializers[std::type_index(typeid( evJoinRejected    ))] = &CStreamCtrlXml::sendEvJoinRejected;
	type_serializers[std::type_index(typeid( evLeave           ))] = &CStreamCtrlXml::sendEvLeave;
	type_serializers[std::type_index(typeid( evGetSessionState ))] = &CStreamCtrlXml::sendEvGetSessionState;
	type_serializers[std::type_index(typeid( evSessionState    ))] = &CStreamCtrlXml::sendEvSessionState;
	type_serializers[std::type_index(typeid( evResetStream     ))] = &CStreamCtrlXml::sendEvResetStream;
	type_serializers[std::type_index(typeid( evSyncStream      ))] = &CStreamCtrlXml::sendEvSyncStream;
	type_serializers[std::type_index(typeid( evSetVolume       ))] = &CStreamCtrlXml::sendEvSetVolume;
	type_serializers[std::type_index(typeid( evAck             ))] = &CStreamCtrlXml::sendEvAck;
	type_serializers[std::type_index(typeid( evError           ))] = &CStreamCtrlXml::sendEvError;


	reset();
}

CStreamCtrlXml::~CStreamCtrlXml() {
	XML_ParserFree(m_parser);
}

void CStreamCtrlXml::setup() {
	sendData(StreamCtrlXMLCmds::open);
}

void CStreamCtrlXml::shutdown() {
	sendData(StreamCtrlXMLCmds::close);
	reset();
}



void CStreamCtrlXml::sendEvent(CmdStreamBase* ev) {
	try {
		((*this).*type_serializers[type_index( typeid(*ev) ) ])(ev);
	}
	catch(std::bad_cast bc) {
		// cerr << "bad cast: " << bc << endl;;
	}
}

//void CStreamCtrlXml::onRecvEvent(CmdStreamBase* ev) {
//}

void CStreamCtrlXml::sendEvClientState(CmdStreamBase* ev) {
	evClientState* e = dynamic_cast<evClientState*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " member_of_session=\"" << e->m_member_of_session << "\""
											 << " session_srv=\"" << e->m_session_srv.to_string() << "\""
											 << " volume=\"" << e->m_current_volume << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvRequestJoin(CmdStreamBase* ev) {
	evRequestJoin* e = dynamic_cast<evRequestJoin*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\""
											<< " mcast_addr=\"" << e->m_mcast_addr.to_string() << "\""
											<< " timesrv_port=\"" << e->m_timesrv_port << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvJoinAccepted(CmdStreamBase* ev) {
	evJoinAccepted* e = dynamic_cast<evJoinAccepted*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " former_session=\"" << e->m_former_session << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvJoinRejected(CmdStreamBase* ev) {
	evJoinRejected* e = dynamic_cast<evJoinRejected*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " owner_session=\"" << e->m_owner_session << "\""
											<< " message=\"" << e->m_message << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvLeave(CmdStreamBase* ev) {
	evLeave* e = dynamic_cast<evLeave* >(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " triggered_by_session=\"" << e->m_triggered_by_session << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvGetSessionState(CmdStreamBase* ev) {
	evGetSessionState* e = dynamic_cast<evGetSessionState*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvSessionState(CmdStreamBase* ev) {
	evSessionState* e = dynamic_cast<evSessionState*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\""
											<< " mcast_addr=\"" << e->m_mcast_addr.to_string() << "\""
											<< " timesrv_port=\"" << e->m_timesrv_port << "\""
											<< " rtp_unicast_port=\"" << e->m_rtp_unicast_port << "\""
											<< " volume=\"" << e->m_volume << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvResetStream(CmdStreamBase* ev) {
	evResetStream* e = dynamic_cast<evResetStream*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " ssrc=\"" << e->m_ssrc << "\""
											<< " rtp_ts=\"" << e->m_rtp_ts << "\""
											<< " media_clock_pts=\"" << e->m_media_clock_pts << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvSyncStream(CmdStreamBase* ev) {
	evSyncStream* e = dynamic_cast<evSyncStream*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " ssrc=\"" << e->m_ssrc << "\""
											<< " rtp_ts=\"" << e->m_rtp_ts << "\""
											<< " media_clock_pts=\"" << e->m_media_clock_pts << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvSetVolume(CmdStreamBase* ev) {
	evSetVolume* e = dynamic_cast<evSetVolume*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " ssrc=\"" << e->m_ssrc << "\""
											<< " volume=\"" << e->m_volume << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvAck(CmdStreamBase* ev) {
	evAck* e = dynamic_cast<evAck*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\"/>" << endl;
	sendData(oss.str());
}


void CStreamCtrlXml::sendEvError(CmdStreamBase* ev) {
	evError* e = dynamic_cast<evError*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " message=\"" << e->m_error_msg << "\"/>" << endl;
	sendData(oss.str());
}


void CStreamCtrlXml::newData(const char* data, int len) {
	int done = 0;
	try {
		if(XML_Parse(m_parser, data, len, done) == XML_STATUS_ERROR) {
			enum XML_Error err = XML_GetErrorCode(m_parser);

			cerr << "error parsing xml data: " << data << " : " << XML_ErrorString(err) << endl;
			// throw exception;
		}
	} catch(ExRpcError rpcEx) {
		cerr << rpcEx.getReason() << endl;
	}
}

void CStreamCtrlXml::sendData(std::string data) {
	onDataToSend(data.c_str(), data.size());
}

void XMLCALL CStreamCtrlXml::startTagHandler(void *inst_ptr, const char *el, const char **attr) {
	CStreamCtrlXml* parser_obj = (CStreamCtrlXml*)inst_ptr;
	parser_obj->onXmlStartElement(std::string(el), attr);
}

void XMLCALL CStreamCtrlXml::endTagHandler(void *inst_ptr, const char *el){
	CStreamCtrlXml* parser_obj = (CStreamCtrlXml*)inst_ptr;
	parser_obj->onXmlEndElement(el);
}

void XMLCALL CStreamCtrlXml::characterHandler(void *inst_ptr, const char *s, int len) {
	CStreamCtrlXml* parser_obj = (CStreamCtrlXml*)inst_ptr;
	parser_obj->onXmlCharacters( string(s, len) );
}


////
//   old stuff
////






//void CStreamCtrlXml::ack(uint32_t cmdID) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::ack        << " cmdID=\"" << cmdID << "\">" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::error(uint32_t cmdID, int errorCode, std::string errmsg) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::error        << " cmdID=\"" << cmdID << "\""
//			                                      << " code=\"" << errorCode << "\""
//												  << " msg=\"" << errmsg << "\""
//                                                  << "/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::joinSession(uint32_t cmdID, std::string name, ip::address session_srv) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::joinSession << " cmdID=\"" << cmdID << "\""
//                                                  << " name=\"" << name << "\""
//			                                      << " addr=\"" << session_srv.to_string() << "\""
//                                                  << ">" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::joinSessionLeave() {
//	ostringstream oss;
//	oss << "</" << StreamCtrlXMLCmds::joinSession << ">" << endl;
//	sendData(oss.str());
//}
//
//
//void CStreamCtrlXml::takeFromSession(uint32_t cmdID, std::string name, ip::address session_srv) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::takeFromSession << " cmdID=\"" << cmdID << "\""
//                                                       << " name=\"" << name << "\""
//													   << " addr=\"" << session_srv.to_string() << "\""
//													   << "/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::setTimeSrv(uint32_t cmdID, boost::asio::ip::address session_srv, uint32_t port) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::setTimeSrv << " cmdID=\"" << cmdID << "\""
//                                                << " addr=\"" << session_srv.to_string() << "\""
//			                                    << " port=\"" << port << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::getTimeSrv(uint32_t cmdID) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::getTimeSrv << " cmdID=\"" << cmdID << "\""
//                                                << "/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::getRTPPort(uint32_t cmdID) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::getRTPPort << " cmdID=\"" << cmdID << "\""
//                                                << "/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::setRTPPort(uint32_t cmdID, uint32_t port) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::setRTPPort << " cmdID=\"" << cmdID << "\""
//                                                << " port=\"" << port << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::joinMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::joinMCastGrp << " cmdID=\"" << cmdID << "\""
//                                                  << " addr=\"" << mcast_addr.to_string() << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::leaveMCastGrp(uint32_t cmdID, boost::asio::ip::address mcast_addr) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::leaveMCastGrp << " cmdID=\"" << cmdID << "\""
//                                                   << " addr=\"" << mcast_addr.to_string() << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::getMCastGrp(uint32_t cmdID) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::getMCastGrp << " cmdID=\"" << cmdID << "\""
//                                                 << "/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::setStreamTimeBase(uint32_t cmdID, uint32_t ssrc, uint64_t rtp_ts, uint64_t pts) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::setStreamTimeBase << " cmdID=\"" << cmdID << "\""
//                                                       << " ssrc=\"" << ssrc << "\""
//			                                           << " rtp_ts=\"" << rtp_ts << "\""
//													   << " pts=\"" << pts << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::getStreamTimeBase(uint32_t cmdID, uint32_t ssrc) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::getStreamTimeBase << " cmdID=\"" << cmdID << "\""
//                                                       << " ssrc=\"" << ssrc << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::resetStream(uint32_t cmdID, uint32_t ssrc) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::resetStream << " cmdID=\"" << cmdID << "\""
//                                                 << " ssrc=\"" << ssrc << "\"/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::getVolume(uint32_t cmdID) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::getVolume << " cmdID=\"" << cmdID << "\""
//                                               << "/>" << endl;
//	sendData(oss.str());
//}
//
//void CStreamCtrlXml::setVolume(uint32_t cmdID, int percent) {
//	ostringstream oss;
//	oss << "<" << StreamCtrlXMLCmds::setVolume << " cmdID=\"" << cmdID << "\""
//                                               << " percent=\"" << percent << "\"/>" << endl;
//	sendData(oss.str());
//}
//


}
