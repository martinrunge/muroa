/*
 * CMuroaXml.cpp
 *
 *  Created on: 12 Aug 2011
 *      Author: martin
 */

#include "CMuroaXml.h"
#include "xmlCommands.h"

#include <sstream>
#include <iostream>

using namespace std;

CMuroaXml::CMuroaXml() throw (ExRpcError)
{

}

CMuroaXml::~CMuroaXml() {
}

void CMuroaXml::open() {

	m_parser = XML_ParserCreate(NULL);
	if (!m_parser) {
		// fprintf(stderr, "Couldn't allocate memory for parser\n");
		ExRpcError ex("no parser");
		throw ex;
	}

	XML_SetUserData(m_parser, (void*) this);
	XML_SetElementHandler(m_parser, CMuroaXml::startTagHandler,	CMuroaXml::endTagHandler);
	XML_SetCharacterDataHandler(m_parser, CMuroaXml::characterHandler);

	sendData(xmlCommands::open);
}

void CMuroaXml::close() {
	sendData(xmlCommands::close);
	reset();
	XML_ParserFree(m_parser);
}

void CMuroaXml::listSessions(vector<string> sessionList) {
	vector<string>::iterator it;
	ostringstream oss;
	oss << "<" << xmlCommands::sessionList << ">" << endl;
	for( it = sessionList.begin(); it != sessionList.end(); it++) {
		oss << "<" << xmlCommands::sessionDesc << " name=\"" << *it << "\"/>" << endl;
	}
	oss << "</" << xmlCommands::sessionList << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::joinSession(string sessionName) {
	ostringstream oss;
	oss << "<" << xmlCommands::joinSession << " name=\"" << sessionName << "\">" << endl;
	sendData(oss.str());
}


void CMuroaXml::leaveSession() {
	sendData(xmlCommands::leaveSession);
}

void CMuroaXml::play(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::play << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::pause(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::pause << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::stop(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::stop << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::next(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::next << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::prev(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::prev << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::stateChanged(int newState) {
	ostringstream oss;
	oss << "<" << xmlCommands::stateChanged << " newState=\"" << newState << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::scanCollection(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::scanCollection << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::progress(uint32_t jobID, int progress, int total) {
	ostringstream oss;
	oss << "<" << xmlCommands::progress << " jobID=\"" << jobID << "\" progress=\"" << progress << "\"" << " total=\"" << total << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::finished(uint32_t jobID) {
	ostringstream oss;
	oss << "<" << xmlCommands::finished << " jobID=\"" << jobID << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::error(uint32_t jobID, int errorCode, std::string description) {
	ostringstream oss;
	oss << "<" << xmlCommands::error << " jobID=\"" << jobID << "\" errorCode=\"" << errorCode << "\">";
	oss << description;
	oss << "</" << xmlCommands::error << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::getCollection(uint32_t jobID, unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getCollection << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::getPlaylist(uint32_t jobID, unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getPlaylist << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::getNextlist(uint32_t jobID, unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getNextlist << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}

void CMuroaXml::getSessionState(uint32_t jobID, unsigned knownRev) {
	ostringstream oss;
	oss << "<" << xmlCommands::getSessionState << " knownRev=\"" << knownRev << "\"/>" << endl;
	sendData(oss.str());
}


void CMuroaXml::editCollection(uint32_t jobID, unsigned fromRev, unsigned toRev, string diff ) {
	ostringstream oss;
	oss << "<" << xmlCommands::editCollection << " fromRev=\"" << fromRev << "\" toRev=\"" << toRev << "\">" << endl;
	oss << "<![CDATA[" << endl;
	oss << diff << endl;
	oss << "]]>" << endl;
	oss << "</" << xmlCommands::editCollection << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::editPlaylist(uint32_t jobID, unsigned fromRev, unsigned toRev, string diff ) {
	ostringstream oss;
	oss << "<" << xmlCommands::editPlaylist << " fromRev=\"" << fromRev << "\" toRev=\"" << toRev << "\">" << endl;
	oss << "<![CDATA[" << endl;
	oss << diff << endl;
	oss << "]]>" << endl;
	oss << "</" << xmlCommands::editPlaylist << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::editNextlist(uint32_t jobID, unsigned fromRev, unsigned toRev, string diff ) {
	ostringstream oss;
	oss << "<" << xmlCommands::editNextlist << " fromRev=\"" << fromRev << "\" toRev=\"" << toRev << "\">" << endl;
	oss << "<![CDATA[" << endl;
	oss << diff << endl;
	oss << "]]>" << endl;
	oss << "</" << xmlCommands::editNextlist << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::editSessionState(uint32_t jobID, unsigned fromRev, unsigned toRev, string diff ) {
	ostringstream oss;
	oss << "<" << xmlCommands::editSessionState << " fromRev=\"" << fromRev << "\" toRev=\"" << toRev << "\">" << endl;
	oss << "<![CDATA[" << endl;
	oss << diff << endl;
	oss << "]]>" << endl;
	oss << "</" << xmlCommands::editSessionState << ">" << endl;
	sendData(oss.str());
}

void CMuroaXml::getSessionClients(uint32_t jobID) {
	ostringstream oss;
}

void CMuroaXml::getUnassignedClients(uint32_t jobID) {
	ostringstream oss;
}

void CMuroaXml::addClient(uint32_t jobID, std::string name) {
	ostringstream oss;
}

void CMuroaXml::rmClient(uint32_t jobID, std::string name) {
	ostringstream oss;
}

void CMuroaXml::enableClient(uint32_t jobID, std::string name) {
	ostringstream oss;
}

void CMuroaXml::disableClient(uint32_t jobID, std::string name) {
	ostringstream oss;
}



void CMuroaXml::newData(const char* data, int len) {
	int done = 0;
	if(XML_Parse(m_parser, data, len, done) == XML_STATUS_ERROR) {
		enum XML_Error err = XML_GetErrorCode(m_parser);

	    cerr << "error parsing xml data: " << data << " : " << XML_ErrorString(err) << endl;
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
