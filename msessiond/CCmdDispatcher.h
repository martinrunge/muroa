/*
 * CCmdDispatcher.h
 *
 *  Created on: 19 Mar 2012
 *      Author: martin
 */

#ifndef CCMDDISPATCHER_H_
#define CCMDDISPATCHER_H_


namespace muroa {
	class Cmd;
	class CSession;
};

class CCmdDispatcher {
public:
	CCmdDispatcher(muroa::CSession* session);
	virtual ~CCmdDispatcher();

	void incomingCmd(muroa::Cmd* cmd);
private:
	muroa::CSession* m_session;
};

#endif /* CCMDDISPATCHER_H_ */
