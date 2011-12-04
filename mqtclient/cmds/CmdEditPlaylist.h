/*
 * CPlaylistCommand.h
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#ifndef CMD_EDIT_PLAYLIST_H_
#define CMD_EDIT_PLAYLIST_H_

#include "CmdBase.h"

class CmdEditPlaylist: public CmdBase {
public:
	CmdEditPlaylist(unsigned knownRev, QObject * parent = 0 );
	CmdEditPlaylist(unsigned knownRev, const std::string& diff, QObject * parent = 0 );
	virtual ~CmdEditPlaylist();

	void setData(const std::string& diff);

	void timeout();
};

#endif /* CMD_EDIT_PLAYLIST_H_ */
