/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
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
#ifndef CIPV4ADDRESS_H
#define CIPV4ADDRESS_H

#include <netinet/in.h>

#include <cipaddress.h>

/**
Class for IPv4 Addresses

@author Martin Runge
*/
class CIPv4Address : public CIPAddress
{
public:
    CIPv4Address();

    CIPv4Address(std::string host, unsigned short port = 0);

    ~CIPv4Address();

    /*!
        \fn CIPv4Address::sock_addr_in_ptr()
     */
    inline struct sockaddr_in* sock_addr_in_ptr()
    {
        return &m_sockaddr_in;
    }
    /** return ip address in dotted notation */
    std::string ipAddress();
    
    /** returns port number in host byte order */
    unsigned short port();

private:
    void fillSockaddrIn(std::string host, unsigned short port);

    int seperateHostAndPort(const std::string& hostandport, std::string* host, unsigned short* port);


    struct sockaddr_in m_sockaddr_in;   

};

#endif
