/*
 Copyright (c) 2002-2014 "Martin Runge"

 IClientSMActions.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef ICLIENTSMACTIONS_H_
#define ICLIENTSMACTIONS_H_

namespace muroa {

class CmdStreamBase;

class IClientSMActions {
public:
	IClientSMActions() {};
	virtual ~IClientSMActions() {};

	// virtual void sendEvent(CmdStreamBase* cmd) = 0;

	virtual void sendClientState() = 0;
	virtual void sendSessionState(const evRequestSessionState& rss) = 0;
	virtual void sendEvSessionError(const evSessionError& evt) = 0;
	virtual void sendEvError(const evError& evt) = 0;
	virtual void sendEvTimeout(const evTimeout& evt) = 0;
    virtual void sendEvVolume(const evVolume &evt) = 0;
    virtual void sendEvt(const CmdStreamBase &evt) = 0;

	virtual void confirmJoinRequest(const evRequestJoin& evt ) = 0;
	virtual bool mayJoinSession(const evRequestJoin& rj) = 0;
	virtual void becomeSessionMember(const evRequestJoin& evt) = 0;
	virtual void leaveSession(const evRequestLeave& evt) = 0;
	virtual void onSyncInfo(const evSyncStream& evt) = 0;
	virtual void onSetVolume(const evSetVolume& evt) = 0;
	virtual void onResetStream(const evResetStream& evt) = 0;

	virtual void onGetSMState(const evGetSMState& evt) = 0;
	virtual void sendSMState(const evSMState& evt) = 0;
	virtual void onBecomeMediaProvider(const evBecomeMediaProvider& evt) = 0;
	virtual void onRevokeMediaProvider(const evRevokeMediaProvider& evt) = 0;

	virtual void sendRejectJoin(const evRequestJoin& evt) = 0;
	virtual void sendRejectJoin(const evLeave& evt) = 0;
	virtual void sendRejectJoin(const evTimeout& evt) = 0;
};

} /* namespace muroa */

#endif /* ICLIENTSMACTIONS_H_ */
