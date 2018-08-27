/*
Copyright (c) 2002-2014 "Martin Runge"

CmdStream.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CmdStream.h"

namespace muroa {

const std::string evClientState::ev_name = "evClientState";
const std::string evRequestJoin::ev_name = "evRequestJoin";
const std::string evRequestLeave::ev_name = "evRequestLeave";
const std::string evJoinAccepted::ev_name = "evJoinAccepted";
const std::string evJoinRejected::ev_name = "evJoinRejected";
const std::string evSessionError::ev_name = "evSessionError";
const std::string evLeave::ev_name = "evLeave";
const std::string evGetSessionState::ev_name ="evGetSessionState";
const std::string evSessionState::ev_name = "evSessionState";
const std::string evResetStream::ev_name = "evResetStream";
const std::string evSyncStream::ev_name = "evSyncStream";
const std::string evSetVolume::ev_name = "evSetVolume";
const std::string evVolume::ev_name = "evVolume";
const std::string evRequestProv::ev_name = "evRequestProv";
const std::string evProvAck::ev_name = "evProvAck";
const std::string evProvRej::ev_name = "evProvRej";
const std::string evRevokeProv::ev_name = "evRevokeProv";
const std::string evAck::ev_name = "evAck";
const std::string evError::ev_name = "evError";

}
