/*
Copyright (c) 2002-2014 "Martin Runge"

cmds.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CMDS_CMDS_H_
#define LIBMSTREAM_CMDS_CMDS_H_

#include "CmdStreamBase.h"

namespace muroa {

class evClientState: public CmdStreamBase {

};

class evRequestJoin: public CmdStreamBase {

};


class evJoinAccepted: public CmdStreamBase {

};

class evJoinRejected: public CmdStreamBase {

};

class evLeave: public CmdStreamBase {

};

class evGetSessionState: public CmdStreamBase {

};

class evSessionState: public CmdStreamBase {

};


class evResetStream: public CmdStreamBase {

};

class evSyncStream: public CmdStreamBase {

};

class evSetVolume: public CmdStreamBase {

};


class evError: public CmdStreamBase {

};

class evTimeout: public CmdStreamBase {

};


}
#endif /* LIBMSTREAM_CMDS_CMDS_H_ */
