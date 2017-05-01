 /**************************************************************************
 *                                                                         *
 *   StreamCtrlXMLCmds.cpp                                                 *
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

#include "StreamCtrlXMLCmds.h"


using namespace std;

namespace muroa {

StreamCtrlXMLCmds::StreamCtrlXMLCmds()
{
}


StreamCtrlXMLCmds::~StreamCtrlXMLCmds()
{
}

const std::string StreamCtrlXMLCmds::open = string("<muroad>\n");
const std::string StreamCtrlXMLCmds::close = string("</muroad>\n");

const std::string StreamCtrlXMLCmds::ack = string("ack");
const std::string StreamCtrlXMLCmds::error = string("error");
const std::string StreamCtrlXMLCmds::volume = string("volume");

const std::string StreamCtrlXMLCmds::requestInfo = string("requestInfo");
const std::string StreamCtrlXMLCmds::getState = string("getState");

const std::string StreamCtrlXMLCmds::joinSession = string("joinSession");

// allowed after client was successfully added to a session
const std::string StreamCtrlXMLCmds::takeFromSession = string("takeFromSession");

const std::string StreamCtrlXMLCmds::getTimeSrv = string("getTimeSrv");
const std::string StreamCtrlXMLCmds::setTimeSrv = string("setTimeSrv");
const std::string StreamCtrlXMLCmds::getRTPPort = string("getRTPPort");
const std::string StreamCtrlXMLCmds::setRTPPort = string("setRTPPort");
const std::string StreamCtrlXMLCmds::joinMCastGrp = string("joinMCastGrp");
const std::string StreamCtrlXMLCmds::leaveMCastGrp = string("leaveMCastGrp");
const std::string StreamCtrlXMLCmds::getMCastGrp = string("getMCastGrp");
const std::string StreamCtrlXMLCmds::setStreamTimeBase = string("setStreamTimeBase");
const std::string StreamCtrlXMLCmds::getStreamTimeBase = string("getStreamTimeBase");
const std::string StreamCtrlXMLCmds::resetStream = string("resetStream");
const std::string StreamCtrlXMLCmds::getVolume = string("getVolume");
const std::string StreamCtrlXMLCmds::setVolume = string("setVolume");

}
