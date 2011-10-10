/*
 * CMuroaXml.cpp
 *
 *  Created on: 12 Aug 2011
 *      Author: martin
 */

#include "CMuroaXml.h"
#include "xmlCommands.h"

#include <sstream>

using namespace std;

CMuroaXml::CMuroaXml() throw (rpcError)
{
	m_parser = XML_ParserCreate(NULL);
	if (!m_parser) {
		// fprintf(stderr, "Couldn't allocate memory for parser\n");
		rpcError ex("no parser");
		throw ex;
	}

	XML_SetUserData(m_parser, (void*) this);
	XML_SetElementHandler(m_parser, CMuroaXml::startTagHandler,	CMuroaXml::endTagHandler);
	XML_SetCharacterDataHandler(m_parser, CMuroaXml::characterHandler);


}

CMuroaXml::~CMuroaXml() {
}

void CMuroaXml::joinSession(uint32_t sessionID) {
	ostringstream oss;
	oss << "<" << xmlCommands::joinSession << " sessionID=\"" << sessionID << "\">" << endl;
	sendData(oss.str());
}


void CMuroaXml::leaveSession() {
	sendData(xmlCommands::leaveSession);
}

void CMuroaXml::play() {
	sendData(xmlCommands::play);
}

void CMuroaXml::pause() {
	sendData(xmlCommands::pause);
}

void CMuroaXml::stop() {
	sendData(xmlCommands::stop);
}

void CMuroaXml::next() {
	sendData(xmlCommands::next);
}

void CMuroaXml::prev() {
	sendData(xmlCommands::prev);
}

void CMuroaXml::stateChanged(int newState) {
	ostringstream oss;
	oss << "<" << xmlCommands::stateChanged << " newState=\"" << newState << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::progress(uint32_t jobID, int progress) {
	ostringstream oss;
	oss << "<" << xmlCommands::progress << " jobID=\"" << jobID << "\" progress=\"" << progress << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::error(uint32_t jobID, int errorCode, std::string description) {
	ostringstream oss;
	oss << "<" << xmlCommands::error << " jobID=\"" << jobID << "\" errorCode=\"" << errorCode << "\">";
	oss << description;
	oss << "</" << xmlCommands::error << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::getCollection(unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getCollection << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::getPlaylist(unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getPlaylist << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::getNextlist(unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getNextlist << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::editCollection(unsigned fromRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::editCollection << " fromRev=\"" << fromRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::editPlaylist(unsigned fromRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::editPlaylist << " fromRev=\"" << fromRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::editNextlist(unsigned fromRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::editNextlist << " fromRev=\"" << fromRev << "\"/>" << endl;
	sendData(oss.str());
}


void CMuroaXml::newData(const char* data, int len) {
	int done = 0;
	if(XML_Parse(m_parser, data, len, done) == XML_STATUS_ERROR) {
	    // throw exception;
	}
}

void CMuroaXml::sendData(std::string data) {
	onDataToSend(data.c_str(), data.size());
}

void XMLCALL CMuroaXml::startTagHandler(void *inst_ptr, const char *el, const char **attr) {
	CMuroaXml* parser_obj = (CMuroaXml*)inst_ptr;
	parser_obj->onStartElement(std::string(el), attr);
}

void XMLCALL CMuroaXml::endTagHandler(void *inst_ptr, const char *el){
	CMuroaXml* parser_obj = (CMuroaXml*)inst_ptr;
	parser_obj->onEndElement(el);
}

void XMLCALL CMuroaXml::characterHandler(void *inst_ptr, const char *s, int len) {
	CMuroaXml* parser_obj = (CMuroaXml*)inst_ptr;
	parser_obj->onCharacters( string(s, len) );
}
