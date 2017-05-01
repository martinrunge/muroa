 /**************************************************************************
 *                                                                         *
 *   IStreamCtrlRPC.h                                                      *
 *                                                                         *
 *   This file is part of libmstream                                       *
 *   Copyright (C) 2014 by Martin Runge <martin.runge@web.de>              *
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


#ifndef ISTREAMCTRLRPC_H_
#define ISTREAMCTRLRPC_H_

#include <string>
#include <vector>
#include <MuroaExceptions.h>
#include <boost/asio.hpp>

namespace muroa {

class CmdStreamBase;

class IStreamCtrl {
public:
	virtual ~IStreamCtrl() {}
	virtual void setup() = 0;
	virtual void shutdown() = 0;

	virtual void sendEvent(const CmdStreamBase* ev) = 0;
	/// return true if event was completely consumed an can be deleted, false, if comumer will take care about delete.
	virtual bool onEvent(CmdStreamBase* ev) = 0;
};

class IStreamCtrlRPC : public IStreamCtrl {
public:
	virtual ~IStreamCtrlRPC() {}

	virtual void setup() = 0;
	virtual void shutdown() = 0;

	virtual void sendEvent(const CmdStreamBase* ev) = 0;
};

class IStreamCtrlCBs : public IStreamCtrl {
public:
	virtual ~IStreamCtrlCBs() {}

	virtual void onSetup() = 0;
	virtual void onShutdown() = 0;

	virtual bool onEvent(CmdStreamBase* ev) = 0;
	virtual void onDataToSend(const char* data, int len) = 0;
};

} // namespace muroa
#endif /* ISTREAMCTRLRPC_H_ */
