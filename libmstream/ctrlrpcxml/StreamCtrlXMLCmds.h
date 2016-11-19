 /**************************************************************************
 *                                                                         *
 *   StreamCtrlXMLCmds.h                                                   *
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

#ifndef XML_COMMANDS_H
#define XML_COMMANDS_H

#include <string>

namespace muroa {

class StreamCtrlXMLCmds {
public:
	StreamCtrlXMLCmds();

    ~StreamCtrlXMLCmds();

    static const std::string open;
    static const std::string close;
    static const std::string ack;
    static const std::string error;
    static const std::string requestInfo;
    static const std::string getState;
    static const std::string joinSession;
    static const std::string takeFromSession;
    static const std::string setTimeSrv;
    static const std::string getTimeSrv;
    static const std::string getRTPPort;
    static const std::string setRTPPort;
    static const std::string joinMCastGrp;
    static const std::string leaveMCastGrp;
    static const std::string getMCastGrp;
    static const std::string setStreamTimeBase;
    static const std::string getStreamTimeBase;
    static const std::string resetStream;
    static const std::string getVolume;
    static const std::string setVolume;

};

};

#endif
