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
	type_serializers[std::type_index(typeid( evRequestLeave    ))] = &CStreamCtrlXml::sendEvRequestLeave;
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



void CStreamCtrlXml::sendEvent(const CmdStreamBase* ev) {
	try {
		((*this).*type_serializers[type_index( typeid(*ev) ) ])(ev);
	}
	catch(std::bad_cast bc) {
		// cerr << "bad cast: " << bc << endl;;
	}
}

//void CStreamCtrlXml::onRecvEvent(CmdStreamBase* ev) {
//}

void CStreamCtrlXml::sendEvClientState(const CmdStreamBase* ev) {
	const evClientState* e = dynamic_cast<const evClientState*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " member_of_session=\"" << e->m_member_of_session << "\""
											 << " session_srv=\"" << e->m_session_srv.to_string() << "\""
											 << " volume=\"" << e->m_current_volume << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvRequestJoin(const CmdStreamBase* ev) {
	const evRequestJoin* e = dynamic_cast<const evRequestJoin*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\""
											<< " mcast_addr=\"" << e->m_mcast_addr.to_string() << "\""
											<< " timesrv_port=\"" << e->m_timesrv_port << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvRequestLeave(const CmdStreamBase* ev) {
	const evRequestLeave* e = dynamic_cast<const evRequestLeave*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\""
											<< " triggered_by=\"" << e->m_triggered_by_name << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvJoinAccepted(const CmdStreamBase* ev) {
	const evJoinAccepted* e = dynamic_cast<const evJoinAccepted*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " former_session=\"" << e->m_former_session << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvJoinRejected(const CmdStreamBase* ev) {
	const evJoinRejected* e = dynamic_cast<const evJoinRejected*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " owner_session=\"" << e->m_owner_session << "\""
											<< " message=\"" << e->m_message << "\"/>" << endl;

	sendData(oss.str());
}

void CStreamCtrlXml::sendEvLeave(const CmdStreamBase* ev) {
	const evLeave* e = dynamic_cast<const evLeave* >(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " triggered_by=\"" << e->m_triggered_by_session << "\""
											<< " member_of_session=\"" << e->m_member_of_session << "\""
											<< " session_srv=\"" << e->m_session_srv.to_string() << "\""
											<< " volume=\"" << e->m_current_volume << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvGetSessionState(const CmdStreamBase* ev) {
	const evGetSessionState* e = dynamic_cast<const evGetSessionState*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvSessionState(const CmdStreamBase* ev) {
	const evSessionState* e = dynamic_cast<const evSessionState*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " session_name=\"" << e->m_session_name << "\""
											<< " mcast_addr=\"" << e->m_mcast_addr.to_string() << "\""
											<< " timesrv_port=\"" << e->m_timesrv_port << "\""
											<< " rtp_unicast_port=\"" << e->m_rtp_unicast_port << "\""
											<< " volume=\"" << e->m_volume << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvResetStream(const CmdStreamBase* ev) {
	const evResetStream* e = dynamic_cast<const evResetStream*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " ssrc=\"" << e->m_ssrc << "\""
											<< " rtp_ts=\"" << e->m_rtp_ts << "\""
											<< " media_clock_pts=\"" << e->m_media_clock_pts << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvSyncStream(const CmdStreamBase* ev) {
	const evSyncStream* e = dynamic_cast<const evSyncStream*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " ssrc=\"" << e->m_ssrc << "\""
											<< " rtp_ts=\"" << e->m_rtp_ts << "\""
											<< " num_channels=\"" << e->m_num_channels << "\""
											<< " sample_rate=\"" << e->m_sample_rate << "\""
											<< " sample_size=\"" << e->m_sample_size << "\""
											<< " media_clock_pts=\"" << e->m_media_clock_pts << "\""
											<< " utc_media_clock_pts=\"" << e->m_utc_media_clock_pts << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvSetVolume(const CmdStreamBase* ev) {
	const evSetVolume* e = dynamic_cast<const evSetVolume*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\""
                                            << " ssrc=\"" << e->m_ssrc << "\""
											<< " volume=\"" << e->m_volume << "\"/>" << endl;
	sendData(oss.str());
}

void CStreamCtrlXml::sendEvAck(const CmdStreamBase* ev) {
	const evAck* e = dynamic_cast<const evAck*>(ev);
	ostringstream oss;
	oss << "<" << e->ev_name << " cmdID=\"" << e->getID() << "\"/>" << endl;
	sendData(oss.str());
}


void CStreamCtrlXml::sendEvError(const CmdStreamBase* ev) {
	const evError* e = dynamic_cast<const evError*>(ev);
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
