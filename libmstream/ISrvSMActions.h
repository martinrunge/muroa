/*
 * ISrvSMActions.h
 *
 *  Created on: 12.09.2015
 *      Author: martin
 */

#ifndef LIBMSTREAM_ISRVSMACTIONS_H_
#define LIBMSTREAM_ISRVSMACTIONS_H_

#include <string>

namespace muroa {

class CmdStreamBase;

class ISrvSMActions {
public:
	ISrvSMActions() {};
	virtual ~ISrvSMActions() {};

	// called by statemachine to inform server (overloaded implementation in derived class is the end receiver)
	virtual void onError(const evJoinRejected* evt) = 0;
	virtual void ontError(const evError* evt) = 0;
	virtual void onTimeout(const evTimeout* evt) = 0;

	virtual void onClientState(const evClientState* evt) = 0;
	virtual void onClientState(const evLeave* evt) = 0;

	// call by statemachine to send message to client
	virtual void sendJoinRequest(const evRequestJoin* evt) = 0;
	virtual void sendLeaveRequest(const evRequestLeave* evt) = 0;
	virtual void sendAck(const evJoinAccepted* evt) = 0;

	virtual void onBecameSessionMember(const CmdStreamBase *cmd) = 0;
	virtual void onLeftSession(const CmdStreamBase *cmd) = 0;
	virtual void onJoinRejected(const CmdStreamBase *cmd) = 0;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_ISRVSMACTIONS_H_ */
