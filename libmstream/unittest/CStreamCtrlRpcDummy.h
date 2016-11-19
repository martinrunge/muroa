/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamCtrl.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef CSTREAMCTRL_H_
#define CSTREAMCTRL_H_

#include "ctrlrpcxml/IStreamCtrlRPC.h"
#include "ctrlrpcxml/CStreamCtrlXml.h"
namespace muroa {

class CStreamCtrlRpcDummy : public CStreamCtrlXml
{
public:
	CStreamCtrlRpcDummy();
	CStreamCtrlRpcDummy(CStreamCtrlRpcDummy* receiver);

	void setReceiver(CStreamCtrlRpcDummy* receiver) { m_receiver = receiver; };

	virtual ~CStreamCtrlRpcDummy();

	void onSetup();
    void onShutdown();

    bool onEvent(muroa::CmdStreamBase* ev);

	// onDataToSend:
	// called when there are serialized command ready to be sent to receiver
	void onDataToSend(const char* data, int len);


	CmdStreamBase*& getLastEv() {
		return m_last_ev;
	}

private:
	CStreamCtrlRpcDummy* m_receiver;

	CmdStreamBase* m_last_ev;
};

} /* namespace muroa */

#endif /* CSTREAMCTRL_H_ */
