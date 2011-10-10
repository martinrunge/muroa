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
#ifndef CPARSERSTATEMACHINE_H
#define CPARSERSTATEMACHINE_H

/**
state machine for the command parser

@author Martin Runge
 */

#include <string>
#include <vector>


enum root_state_t {   ROOT_STATE,
					  IN_SESSION_STATE };

enum session_state_t {SESSION_ROOT_STATE,
	                  IN_PLAY,
	                  IN_PAUSE,
	                  IN_STOP,
	                  IN_NEXT,
	                  IN_PREV,
	                  IN_PROGRESS,
	                  IN_STATE_CHANGED,
	                  IN_ERROR,
                      IN_GET_COLLECTION_STATE,
                      IN_GET_PLAYLIST_STATE,
                      IN_GET_NEXTLIST_STATE,
                      IN_COLLECTION_STATE,
                      IN_PLAYLIST_STATE,
                      IN_NEXTLIST_STATE,
                      IN_EDIT_COLLECTION_STATE,
                      IN_EDIT_PLAYLIST_STATE,
                      IN_EDIT_NEXTLIST_STATE,
                      IN_LEAVE,
};

typedef struct {
	root_state_t root_state;
	session_state_t session_state;
} parser_state;


class CParserStateMachine{
public:
	CParserStateMachine();

	virtual ~CParserStateMachine();

	void onStartDocument();
	void onEndDocument();
	void onStartElement(const std::string& name, const char** attributes);
	void onEndElement(const std::string& name);
	void onCharacters(const std::string& text);
	void onComment(const std::string& text);
	void onWarning(const std::string& text);
	void onError(const std::string& text);

	virtual void onJoinSession(uint32_t sessionID) = 0;
	virtual void onLeaveSession() = 0;

	virtual void onPlay() = 0;
	virtual void onStop() = 0;
	virtual void onPause() = 0;
	virtual void onNext() = 0;
	virtual void onPrev() = 0;

	virtual void onStateChanged(int newState) = 0;
	virtual void onProgress(uint32_t jobID, int progress) = 0;
	virtual void onError(uint32_t jobID, int errorCode, std::string description) = 0;

	virtual void onGetCollection( unsigned knownRev ) = 0;
	virtual void onGetPlaylist( unsigned knownRev ) = 0;
	virtual void onGetNextlist( unsigned knownRev ) = 0;

	virtual void onCollection( unsigned knownRev, std::string collection ) = 0;
	virtual void onPlaylist( unsigned knownRev, std::string playlist ) = 0;
	virtual void onNextlist( unsigned knownRev, std::string nextlist ) = 0;

	virtual void onEditCollection( unsigned fromRev, std::string editScript ) = 0;
	virtual void onEditPlaylist( unsigned fromRev, std::string editScript ) = 0;
	virtual void onEditNextlist( unsigned fromRev, std::string editScript ) = 0;

	virtual void onStartSession() = 0;
	virtual void onEndSession() = 0;
	virtual void onXmlVersion() = 0;


private:
	std::string m_concatenate_dummy;

	enum action_flag { INIT, START, END};

	parser_state m_state;

	int m_xml_tag_depth;
	int m_tag_unknown_depth;
	bool m_in_unknown_tag;

	bool m_concatenate_chars_state;

	unsigned m_knownRev;
	unsigned m_fromRev;
	unsigned m_diffFromRev;

	uint32_t m_jobID;
	int m_progress;
	int m_newState;
	int m_errorCode;
	std::string m_errorDescription;
	std::string m_get_text;
	std::string m_edit_text;

	int sessionState(const action_flag& init_start_end, const std::string& name, const char** attrs);
	int processSessionState(const action_flag& init_start_end, const std::string& name, const char** attrs);

	int parseJoinArgs(const char** attrs);

	void parseNextArgs(const char** attrs);
    void parsePrevArgs(const char** attrs);
    void parseStopArgs(const char** attrs);
    void parsePlayArgs(const char** attrs);

	void parseProgressArgs(const char** attrs);
	void parseStateChangedArgs(const char** attrs);
	void parseErrorArgs(const char** attrs);

    void parseKnownRev(const char** attrs);
    void parseFromRev(const char** attrs);
    void parseDiffFromRev(const char** attrs);

};

#endif
