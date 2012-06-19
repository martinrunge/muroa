/***************************************************************************
 *   Copyright (C) 2004 by Martin Runge                                    *
 *   martin.runge@web.de                                                   *
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

#include "CParserStateMachine.h"
#include "CUtils.h"
#include "muroaxml/xmlCommands.h"

#include <iostream>
#include <assert.h>
using namespace std;


CParserStateMachine::CParserStateMachine()
{
	reset();
}

void CParserStateMachine::reset() {
	m_concatenate_chars_state = false;
	m_concatenate_dummy.erase();
	// m_command_connection = parent;
	// m_session = session;

	m_state.root_state = ROOT_STATE;
	m_state.session_state = SESSION_ROOT_STATE;
	m_xml_tag_depth = 0;
	m_tag_unknown_depth = 0;
	m_in_unknown_tag = false;
}

CParserStateMachine::~CParserStateMachine()
{
}


void CParserStateMachine::onStartDocument()
{
	std::cerr << "on_start_document()" << std::endl;
}

void CParserStateMachine::onEndDocument()
{
	std::cerr << "on_end_document()" << std::endl;
}

void CParserStateMachine::onStartElement(const std::string& name, const char** attributes)
{
	std::cerr << "start node name=" << name << std::endl;

	// Print attributes:
	for(int i=0; attributes[i]; i+=2)
	{
		std::cerr << attributes[i] << " = " << attributes[i + 1] << std::endl;
	}
	switch (m_state.root_state) {
		case IN_SESSION_STATE:
			// we reached root state (for root state this can only happen at at document start)
			sessionState(START, name, attributes);
			break;

		case IN_SESSION_LIST_STATE:
			sessionListState(START, name, attributes);
			break;

		case ROOT_STATE:
		default:

			// the state machine is in root state. Whatever can happen from here is handeled by the function 'SessionState'
			if(name.compare(xmlCommands::joinSession) == 0) {
				sessionState(INIT, name, attributes);
			}
			if(name.compare(xmlCommands::sessionList) == 0) {
				sessionListState(INIT, name, attributes);
			}
			break;
	}
	return;
}

void CParserStateMachine::onEndElement(const std::string& name)
{
	std::cerr << "on_end_element()" << std::endl;
	const char **null_ptr( 0 );

	switch (m_state.root_state) {
		case IN_SESSION_STATE:
			// we reached root state (for root state this can only happen at at document start)
			sessionState(END, name, null_ptr);
			break;

		case IN_SESSION_LIST_STATE:
			sessionListState(END, name, null_ptr);
			break;

		case ROOT_STATE:
		default:

			// the state machine is in root state. Whatever can happen from here is handeled by the function 'SessionState'
			if(name.compare(xmlCommands::joinSession) == 0) {
				if(name.compare("session") == 0) {
					m_state.root_state = ROOT_STATE;
				}
			}
			if(name.compare(xmlCommands::sessionList) == 0) {
				m_state.root_state = ROOT_STATE;
			}
			break;
	}
	return;
}

void CParserStateMachine::onCharacters(const std::string& text)
{
	// std::cerr << "on_characters(): " << text << std::endl;
	switch(m_state.session_state) {
	case IN_ERROR:
		m_errorDescription += text;
		break;

	case IN_COLLECTION_STATE:
	case IN_PLAYLIST_STATE:
	case IN_NEXTLIST_STATE:
		m_get_text += text;
		break;
	case IN_EDIT_COLLECTION_STATE:
	case IN_EDIT_PLAYLIST_STATE:
	case IN_EDIT_NEXTLIST_STATE:
		m_edit_text += text;
		break;
	default:
		std::cerr << "unexpected characters: " << text << std::endl;

	}
}

void CParserStateMachine::onComment(const std::string& text)
{
	std::cerr << "on_comment(): " << text << std::endl;
}

void CParserStateMachine::onWarning(const std::string& text)
{
	std::cerr << "on_warning(): " << text << std::endl;
}

void CParserStateMachine::onError(const std::string& text)
{
	std::cerr << "on_error(): " << text << std::endl;
}


int CParserStateMachine::sessionState(const action_flag& init_start_end, const std::string& name, const char** attrs) {

	std::cerr << "CParserStateMachine::sessionState" << std::endl;
	switch (init_start_end) {
	case INIT:
		m_xml_tag_depth = 1;
		parseJoinArgs(attrs);
		m_state.root_state = IN_SESSION_STATE;
		break;

	case START:
		if (name.compare("play") == 0) {
			m_state.session_state = IN_PLAY;
		}
		else if (name.compare("pause") == 0) {
			m_state.session_state = IN_PAUSE;
		}
		else if (name.compare("next") == 0) {
			m_state.session_state = IN_NEXT;
		}
		else if (name.compare("prev") == 0) {
			m_state.session_state = IN_PREV;
		}
		else if (name.compare("stop") == 0) {
			m_state.session_state = IN_STOP;
		}
		else if (name.compare("pause") == 0) {
			m_state.session_state = IN_PAUSE;
		}
		else if (name.compare("leave") == 0) {
			m_state.session_state = IN_LEAVE;
		}
		else if (name.compare(xmlCommands::scanCollection) == 0) {
			m_state.session_state = IN_SCAN_COLLECTION;
			m_jobID = parseJobID(attrs);
		}
		else if (name.compare(xmlCommands::progress) == 0) {
			m_state.session_state = IN_PROGRESS;
			parseProgressArgs(attrs);
		}
		else if (name.compare("stateChanged") == 0) {
			m_state.session_state = IN_STATE_CHANGED;
			parseStateChangedArgs(attrs);
		}
		else if (name.compare("error") == 0) {
			m_state.session_state = IN_ERROR;
			parseErrorArgs(attrs);
		}
		// get
		else if (name.compare("getCollection") == 0) {
			m_state.session_state = IN_GET_COLLECTION_STATE;
			parseKnownRev(attrs);   // init with attrs
		}
		else if (name.compare("getPlaylist") == 0) {
			m_state.session_state = IN_GET_PLAYLIST_STATE;
			parseKnownRev(attrs);   // init with attrs
		}
		else if (name.compare("getNextlist") == 0) {
			m_state.session_state = IN_GET_NEXTLIST_STATE;
			parseKnownRev(attrs);   // init with attrs
		}
		// response to get
		else if (name.compare("collection") == 0) {
			m_state.session_state = IN_COLLECTION_STATE;
			parseDiffFromRev(attrs);   // init with attrs
		}
		else if (name.compare("playlist") == 0) {
			m_state.session_state = IN_PLAYLIST_STATE;
			parseDiffFromRev(attrs);   // init with attrs
		}
		else if (name.compare("nextlist") == 0) {
			m_state.session_state = IN_NEXTLIST_STATE;
			parseDiffFromRev(attrs);   // init with attrs
		}
		// edit
		else if (name.compare("editCollection") == 0) {
			m_state.session_state = IN_EDIT_COLLECTION_STATE;
			parseFromRev(attrs);   // init with attrs
			parseToRev(attrs);   // init with attrs
		}
		else if (name.compare("editPlaylist") == 0) {
			m_state.session_state = IN_EDIT_PLAYLIST_STATE;
			parseFromRev(attrs);   // init with attrs
			parseToRev(attrs);   // init with attrs
		}
		else if (name.compare("editNextlist") == 0) {
			m_state.session_state = IN_EDIT_NEXTLIST_STATE;
			parseFromRev(attrs);   // init with attrs
			parseToRev(attrs);   // init with attrs
		}
		else if (name.compare("leave") == 0) {
			onLeaveSession();
		}

		else {
			cerr << "CParserStateMachine::processSessionState (START): unknown tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth++;
			m_xml_tag_depth++;
		}
		break;

	case END:
		if(name.compare("session") == 0) {
			// end of session
			m_xml_tag_depth = 0;
			m_state.root_state = ROOT_STATE;
			onLeaveSession();
		}
		else if(name.compare("next") == 0) {
			onNext(m_jobID);
			m_jobID = 0;
		}
		else if(name.compare("prev") == 0) {
			onPrev(m_jobID);
			m_jobID = 0;
		}
		else if(name.compare("play") == 0) {
			onPlay(m_jobID);
			m_jobID = 0;
		}
		else if(name.compare("pause") == 0) {
			onPause(m_jobID);
			m_jobID = 0;
		}
		else if(name.compare("stop") == 0) {
			onStop(m_jobID);
			m_jobID =0;
		}
		else if (name.compare(xmlCommands::scanCollection) == 0) {
			onScanCollection(m_jobID);
		}
		else if (name.compare(xmlCommands::progress) == 0) {
			onProgress(m_jobID, m_progress);
		}
		else if (name.compare("stateChanged") == 0) {
			onStateChanged(m_newState);
		}
		else if (name.compare("error") == 0) {
			onError(m_jobID, m_errorCode, m_errorDescription);
			m_jobID =0;
			m_errorCode = 0;
			m_errorDescription = "";
		}
		// get
		else if (name.compare("getCollection") == 0) {
			// got collection
			onGetCollection(m_jobID, m_knownRev);
			m_jobID= 0;
		}
		else if (name.compare("getPlaylist") == 0) {
			onGetPlaylist(m_jobID, m_knownRev);
			m_jobID= 0;
		}
		else if (name.compare("getNextlist") == 0) {
			onGetNextlist(m_jobID, m_knownRev);
			m_jobID= 0;
		}

		// response to get
		else if (name.compare("collection") == 0) {
			onCollection(m_jobID, m_diffFromRev, m_get_text);
			m_jobID = 0;
			m_get_text = "";
		}
		else if (name.compare("playlist") == 0) {
			onPlaylist(m_jobID, m_diffFromRev, m_get_text);
			m_jobID = 0;
			m_get_text = "";
		}
		else if (name.compare("nextlist") == 0) {
			onNextlist(m_jobID, m_diffFromRev, m_get_text);
			m_jobID = 0;
			m_get_text = "";
		}

		// edit
		else if (name.compare("editCollection") == 0) {
			onEditCollection(m_jobID, m_fromRev, m_toRev, m_edit_text);
			m_jobID = 0;
			m_edit_text = "";
		}
		else if (name.compare("editPlaylist") == 0) {
			onEditPlaylist(m_jobID, m_fromRev, m_toRev, m_edit_text);
			m_jobID = 0;
			m_edit_text = "";
		}
		else if (name.compare("editNextlist") == 0) {
			onEditNextlist(m_jobID, m_fromRev, m_toRev, m_edit_text);
			m_jobID = 0;
			m_edit_text = "";
		}
		else if(m_tag_unknown_depth > 1) {
			m_tag_unknown_depth--;
			m_xml_tag_depth--;
		}
		else {
			cerr << "CParserStateMachine::processSessionState (END): unknown tag received :'" << name << "' !" << endl;
		}
		break;
	}

	return 0;
}


int CParserStateMachine::parseListSessionArgs(const char** attrs) {
	string name, value;
	vector<string> sessionList;

	for(int i=0; attrs[i]; i+=2)
	{
		name = attrs[i];
		value = attrs[i + 1];

		if(name.compare("name") == 0) {
			cerr << name << endl;
			sessionList.push_back(value);
		}
	}
	onListSessions(sessionList);
	return 0;
}

int CParserStateMachine::parseJoinArgs(const char** attrs) {
	string name, value;
	string sessionName;

	for(int i=0; attrs[i]; i+=2)
	{
		name = attrs[i];
		value = attrs[i + 1];

		if(name.compare("name") == 0) {
			cerr << name << endl;
			sessionName = value;
			// get pos to PlaylistAddCall
			cerr << "CParserStateMachine::stateMachine join: sessionName = '" << sessionName << "'" << endl;
		}
	}
	onJoinSession(sessionName);
	if(m_state.root_state == ROOT_STATE) {
		m_state.root_state = IN_SESSION_STATE;
	}
	return 0;
}

uint32_t CParserStateMachine::parseJobID(const char** attrs) {
	assert(attrs[0]);
	assert(attrs[1]);

	uint32_t jobID;
	string name  = attrs[0];
	string value = attrs[1];

	if(name.compare("jobID") == 0) {
		jobID = CUtils::str2uint32(value);
	}

	return jobID;
}


void CParserStateMachine::parseNextArgs(const char **attrs) {
}

void CParserStateMachine::parsePrevArgs(const char **attrs) {
}

void CParserStateMachine::parseStopArgs(const char **attrs) {
}

void CParserStateMachine::parsePlayArgs(const char **attrs) {
}

void CParserStateMachine::parseProgressArgs(const char** attrs) {
	m_jobID = parseJobID(attrs);

	for(int i=2; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("jobID") == 0) {
			m_jobID = CUtils::str2uint32(value);
		}
		else if(name.compare("progress") == 0) {
			m_progress = CUtils::str2long(value);
		}
	}
}

void CParserStateMachine::parseStateChangedArgs(const char** attrs) {
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("newState") == 0) {
			m_newState = CUtils::str2long(value);
		}
	}
}

void CParserStateMachine::parseErrorArgs(const char** attrs) {
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("jobID") == 0) {
			m_jobID = CUtils::str2uint32(value);
		}
		else if(name.compare("errorCode") == 0) {
			m_errorCode = CUtils::str2long(value);
		}
	}
}


void CParserStateMachine::parseKnownRev(const char **attrs) {

	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("knownRev") == 0) {
			cerr << name << endl;
			m_knownRev = CUtils::str2uint32(value);
		}
	}
}

void CParserStateMachine::parseFromRev(const char** attrs) {
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("fromRev") == 0) {
			cerr << name << endl;
			m_fromRev = CUtils::str2uint32(value);
		}
	}
}

void CParserStateMachine::parseToRev(const char** attrs) {
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("toRev") == 0) {
			cerr << name << endl;
			m_toRev = CUtils::str2uint32(value);
		}
	}
}

void CParserStateMachine::parseDiffFromRev(const char** attrs) {
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("diffFromRev") == 0) {
			cerr << name << endl;
			m_diffFromRev = CUtils::str2uint32(value);
		}
	}
}



int CParserStateMachine::sessionListState(const action_flag& init_start_end, const std::string& name, const char** attrs) {
	std::cerr << "CParserStateMachine::sessionListState" << std::endl;
	switch (init_start_end) {
	case INIT:
		m_xml_tag_depth = 1;
		parseSessionListArgs(attrs);
		m_state.root_state = IN_SESSION_LIST_STATE;
		break;

	case START:
		if (name.compare(xmlCommands::sessionDesc) == 0) {
			m_state.session_list_state = IN_DESC;
			parseSessionDescArgs(attrs);
		}
		else {
			cerr << "CParserStateMachine::processSessionListState (START): unknown tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth++;
			m_xml_tag_depth++;
		}
		break;

	case END:
		if(name.compare(xmlCommands::sessionList) == 0) {
			// end of session
			onListSessions( m_sessionList );
			m_xml_tag_depth = 0;
			m_state.root_state = ROOT_STATE;
		}
		else if(name.compare(xmlCommands::sessionDesc) == 0) {
			m_sessionList.push_back(m_tmp_session_name);
		}
		else {
			cerr << "CParserStateMachine::processSessionListState (END): unknown tag received :'" << name << "' !" << endl;
			m_tag_unknown_depth--;
			m_xml_tag_depth--;
		}
		break;
	}

	return 0;
}


void CParserStateMachine::parseSessionListArgs(const char** attrs){
	return;
}


void CParserStateMachine::parseSessionDescArgs(const char** attrs){
	for(int i=0; attrs[i]; i+=2)
	{
		string name  = attrs[i];
		string value = attrs[i + 1];

		if(name.compare("name") == 0) {
			cerr << name << endl;
			m_tmp_session_name = value;
		}
	}
}

