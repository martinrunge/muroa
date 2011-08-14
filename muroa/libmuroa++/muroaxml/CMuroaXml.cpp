/*
 * CMuroaXml.cpp
 *
 *  Created on: 12 Aug 2011
 *      Author: martin
 */

#include "CMuroaXml.h"

CMuroaXml::CMuroaXml() throw(rpcError) {
	  m_parser = XML_ParserCreate(NULL);
	  if (!m_parser) {
	    // fprintf(stderr, "Couldn't allocate memory for parser\n");
		rpcError ex("no parser");
	    throw ex;
	  }

	  XML_SetUserData( m_parser, (void*) this);
	  XML_SetElementHandler(m_parser, CMuroaXml::startTagHandler, CMuroaXml::endTagHandler);
}

CMuroaXml::~CMuroaXml() {
}

void CMuroaXml::join(std::string host, unsigned  port) {
}

void CMuroaXml::leave() {
}

void CMuroaXml::onConnectionStateChanged(int state) {
}

void CMuroaXml::play() {
}

void CMuroaXml::stop() {
}

void CMuroaXml::next() {
}

void CMuroaXml::prev() {
}

void CMuroaXml::getCollection(int knownRev) {
}

void CMuroaXml::getPlaylist(int knownRev) {
}

void CMuroaXml::getNextlist(int knownRev) {
}

void CMuroaXml::editCollection(int fromRev) {
}

void CMuroaXml::editPlaylist(int fromRev) {
}

void CMuroaXml::editNextlist(int fromRev) {
}

void CMuroaXml::onPlay() {
}

void CMuroaXml::onStop() {
}

void CMuroaXml::onNext() {
}

void CMuroaXml::onPrev() {
}

void CMuroaXml::onGetCollection(int knownRev) {
}

void CMuroaXml::onGetPlaylist(int knownRev) {
}

void CMuroaXml::onGetNextlist(int knownRev) {
}

void CMuroaXml::onCollection( unsigned knownRev ) {
}

void CMuroaXml::onPlaylist( unsigned knownRev ) {
}

void CMuroaXml::onNextlist( unsigned knownRev ) {
}

void CMuroaXml::onEditCollection(int fromRev) {
}

void CMuroaXml::onEditPlaylist(int fromRev) {
}

void CMuroaXml::onEditNextlist(int fromRev) {
}

void XMLCALL CMuroaXml::startTagHandler(void *inst_ptr, const char *el, const char **attr) {
	CMuroaXml* parser_obj = (CMuroaXml*)inst_ptr;
	parser_obj->onStartElement(std::string(el), attr);
}

void XMLCALL CMuroaXml::endTagHandler(void *inst_ptr, const char *el){
	CMuroaXml* parser_obj = (CMuroaXml*)inst_ptr;
	parser_obj->onEndElement(el);
}
