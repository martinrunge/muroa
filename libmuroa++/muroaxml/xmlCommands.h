//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Martin Runge <martin.runge@web.de>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XML_COMMANDS_H
#define XML_COMMANDS_H

#include <string>

class xmlCommands{
public:
    xmlCommands();

    ~xmlCommands();

    static const std::string sessionStart;
    static const std::string sessionEnd;
    static const std::string play;
    static const std::string pause;
    static const std::string stop;
    static const std::string next;
    static const std::string prev;
    static const std::string progress;
    static const std::string stateChanged;
    static const std::string error;
    static const std::string XML_ver;
    static const std::string getCollection;
    static const std::string getPlaylist;
    static const std::string getNextlist;
    static const std::string editCollection;
    static const std::string editPlaylist;
    static const std::string editNextlist;
    static const std::string joinSession;
    static const std::string leaveSession;



};

#endif
