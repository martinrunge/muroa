/*
 * CCmdDispatcher.h
 *
 *  Created on: 19 Mar 2012
 *      Author: martin
 */

#ifndef CCMDDISPATCHER_H_
#define CCMDDISPATCHER_H_

#include <MuroaExceptions.h>

namespace muroa {
	class Cmd;
	class CSession;
	class CmdEditPlaylist;
};


class CCmdDispatcher {
public:
	CCmdDispatcher(muroa::CSession* session);
	virtual ~CCmdDispatcher();

	void incomingCmd(muroa::Cmd* cmd);
	void addIdToPlaylistDiff(muroa::CmdEditPlaylist* epl_cmd) throw(MalformedPatchEx);

private:
	muroa::CSession* m_session;
};

#endif /* CCMDDISPATCHER_H_ */
