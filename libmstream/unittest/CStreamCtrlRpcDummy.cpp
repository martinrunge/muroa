/*
 Copyright (c) 2002-2014 "Martin Runge"

 CStreamCtrl.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CStreamCtrlRpcDummy.h"
#include <sstream>

using namespace std;

namespace muroa {

CStreamCtrlRpcDummy::CStreamCtrlRpcDummy() {
	m_last_ev = 0;
}

CStreamCtrlRpcDummy::CStreamCtrlRpcDummy(CStreamCtrlRpcDummy* receiver) : m_receiver(receiver) {
	m_last_ev = 0;
}


CStreamCtrlRpcDummy::~CStreamCtrlRpcDummy() {

}

void CStreamCtrlRpcDummy::onSetup() {
}

void CStreamCtrlRpcDummy::onShutdown() {
}

bool CStreamCtrlRpcDummy::onEvent(muroa::CmdStreamBase* ev) {
	m_last_ev = ev;
	return false;
}

void CStreamCtrlRpcDummy::onDataToSend(const char* data, int len) {
	m_receiver->newData(data, len);
}

} /* namespace muroa */

