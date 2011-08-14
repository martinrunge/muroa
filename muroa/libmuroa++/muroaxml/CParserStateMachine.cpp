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
#include <iostream>
using namespace std;


CParserStateMachine::CParserStateMachine()
{
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
	std::cerr << "node name=" << name << std::endl;

	// Print attributes:
	for(int i=0; attributes[i]; i+=2)
	{
		std::cerr << attributes[i] << " = " << attributes[i + 1] << std::endl;
	}
	if(m_state.root_state == IN_SESSION_STATE) {
		// we reached root state (for root state this can only happen at at document start)
		sessionState(START, name, attributes);
	}
	else {
		// the state machine is in root state. Whatever can happen from here is handeled by the function 'SessionState'
		sessionState(INIT, name, attributes);
	}
	return;
}

void CParserStateMachine::onEndElement(const std::string& name)
{
	std::cerr << "on_end_element()" << std::endl;
	const char **nullptr = 0;

	if(m_state.root_state == ROOT_STATE) {
		// leave root state.
		// handle the data received in root state from here.
	}
	else {
		// we are leaving a element unter root state. This is processed by the function 'RootState'
		sessionState(END, name, nullptr);
	}
	return;

}

void CParserStateMachine::onCharacters(const std::string& text)
{
	std::cerr << "on_characters(): " << text << std::endl;
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


int CParserStateMachine::sessionState(const action_flag& init_start_end, const std::string& name, const char** properties) {

	std::cerr << "CParserStateMachine::sessionState" << std::endl;
	switch (init_start_end) {
	case INIT:
		m_xml_tag_depth = 1;
		m_state.root_state = IN_SESSION_STATE;
		break;

	case START:
		if(name.compare("join") == 0) {
			parseJoinArgs(properties);
		}
		else if (name.compare("play") == 0) {
			m_state.session_state = IN_PLAY;
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
		// get
		else if (name.compare("getCollection") == 0) {
			m_state.session_state = IN_GET_COLLECTION_STATE;
			parseGetCollectionArgs(properties);   // init with properties
		}
		else if (name.compare("getPlaylist") == 0) {
			m_state.session_state = IN_GET_PLAYLIST_STATE;
			parseGetPlaylistArgs(properties);
		}
		else if (name.compare("getNextlist") == 0) {
			m_state.session_state = IN_GET_NEXTLIST_STATE;
			parseGetNextlistArgs(properties);
		}
		// response to get
		else if (name.compare("collection") == 0) {
			m_state.session_state = IN_COLLECTION_STATE;
			parseCollectionArgs(properties);   // init with properties
		}
		else if (name.compare("playlist") == 0) {
			m_state.session_state = IN_PLAYLIST_STATE;
			parsePlaylistArgs(properties);
		}
		else if (name.compare("nextlist") == 0) {
			m_state.session_state = IN_NEXTLIST_STATE;
			parseNextlistArgs(properties);
		}
		// edit
		else if (name.compare("editCollection") == 0) {
			m_state.session_state = IN_EDIT_COLLECTION_STATE;
			parseEditCollectionArgs(properties);   // init with properties
		}
		else if (name.compare("editPlaylist") == 0) {
			m_state.session_state = IN_EDIT_PLAYLIST_STATE;
			parseEditPlaylistArgs(properties);
		}
		else if (name.compare("editNextlist") == 0) {
			m_state.session_state = IN_EDIT_NEXTLIST_STATE;
			parseEditNextlistArgs(properties);
		}
		else if (name.compare("leave") == 0) {
			onLeave();
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

		}
		else if(name.compare("join") == 0) {
			onJoin();
		}
		else if(name.compare("next") == 0) {
			onNext();
		}
		else if(name.compare("prev") == 0) {
			onPrev();
		}
		else if(name.compare("play") == 0) {
			onPlay();
		}
		else if(name.compare("stop") == 0) {
			onStop();
		}
		// get
		else if (name.compare("getCollection") == 0) {
			// got collection
			onGetCollection(m_knownRev);
		}
		else if (name.compare("getPlaylist") == 0) {
			onGetPlaylist(m_knownRev);
		}
		else if (name.compare("getNextlist") == 0) {
			onGetNextlist(m_knownRev);
		}

		// response to get
		else if (name.compare("collection") == 0) {
			onCollection(m_diffFromRev);
		}
		else if (name.compare("playlist") == 0) {
			onPlaylist(m_diffFromRev);
		}
		else if (name.compare("nextlist") == 0) {
			onNextlist(m_diffFromRev);
		}

		// edit
		else if (name.compare("editCollection") == 0) {
			onEditCollection(m_fromRev);
		}
		else if (name.compare("editPlaylist") == 0) {
			onEditPlaylist(m_fromRev);
		}
		else if (name.compare("editNextlist") == 0) {
			onEditNextlist(m_fromRev);
		}
		else if(name.compare("quit") == 0) {
			// end tag of join to land here
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

int CParserStateMachine::parseJoinArgs(const char** properties) {
	long port, token;

	port = 0;
	token = 0;

	string name, value;

	for(int i=0; properties[i]; i+=2)
	{
		name = properties[i];
		value = properties[i + 1];

		if(name.compare("port") == 0) {
			cerr << name << endl;
			port = strtol(value.c_str(), NULL, 10);
			// get pos to PlaylistAddCall
			cerr << "CParserStateMachine::stateMachine join: port = " << port << endl;
		}
		else {
			if(name.compare("token") == 0) {
				cerr << name << endl;
				token = strtol(value.c_str(), NULL, 10);
				cerr << "CParserStateMachine::stateMachine join: token = " << token << endl;
			}
		}
	}
	if(port == 0) {
		onJoin();
	}
	else {
		onJoin(port, token);
	}

	return 0;
}


//void CParserStateMachine::processPlayArgs(const char** properties) {
//	string name, value;
//	int pos_to_play = -1;
//
//	for(int i=0; properties[i]; i+=2)
//	{
//		name = properties[i];
//		value = properties[i + 1];
//
//		if(name.compare("pos") == 0) {
//			cerr << name << endl;
//			pos_to_play = strtol(value.c_str(), NULL, 10);
//			// get pos to PlaylistAddCall
//			cerr << "CParserStateMachine::processPlayArgs: pos_to_play = " << pos_to_play << endl;
//		}
//	}
//	onPlay(pos_to_play);
//}

void CParserStateMachine::parseNextArgs(const char **properties) {
}

void CParserStateMachine::parsePrevArgs(const char **properties) {
}

void CParserStateMachine::parseStopArgs(const char **properties) {
}

void CParserStateMachine::parsePlayArgs(const char **properties) {
}

void CParserStateMachine::parseGetNextlistArgs(const char **properties) {
}

void CParserStateMachine::parseGetPlaylistArgs(const char **properties) {
}

void CParserStateMachine::parseGetCollectionArgs(const char **properties) {
}

void CParserStateMachine::parseNextlistArgs(const char** properties) {
}

void CParserStateMachine::parsePlaylistArgs(const char** properties) {
}

void CParserStateMachine::parseCollectionArgs(const char** properties) {
}

void CParserStateMachine::parseEditNextlistArgs(const char **properties) {
}

void CParserStateMachine::parseEditPlaylistArgs(const char **properties) {
}

void CParserStateMachine::parseEditCollectionArgs(const char **properties) {
}
