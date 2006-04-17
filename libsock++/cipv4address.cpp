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
#include "cipv4address.h"

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>



using namespace std;

CIPv4Address::CIPv4Address()
 : CIPAddress()
{
}

CIPv4Address::CIPv4Address(std::string host, unsigned short port)
 : CIPAddress()
{

  string tmp_host;
  unsigned short tmp_port = 0;
  seperateHostAndPort(host, &tmp_host, &tmp_port);
  
  if(tmp_port == 0) {
    tmp_port = port;
  }

  fillSockaddrIn(tmp_host, tmp_port);
  
    

}



CIPv4Address::~CIPv4Address()
{
}


std::string CIPv4Address::ipAddress() {
  return string(inet_ntoa(m_sockaddr_in.sin_addr));
}


unsigned short CIPv4Address::port() {
  return ntohs(m_sockaddr_in.sin_port);
}

void CIPv4Address::port(unsigned short port_nr) {
  m_sockaddr_in.sin_port = htons(port_nr);

}




void CIPv4Address::fillSockaddrIn(std::string host, unsigned short port) {

  struct hostent *hostinfo;
  
  hostinfo = gethostbyname (host.c_str());
  if (hostinfo == NULL)
    {
      cerr << "Unknown host: " << host << endl;
      return;
    }

  m_sockaddr_in.sin_family = AF_INET;
  m_sockaddr_in.sin_port = htons (port);
  m_sockaddr_in.sin_addr.s_addr = *(reinterpret_cast<in_addr_t*>(hostinfo->h_addr_list[0]));  

}



/*!
    \fn CSocket::seperateHostAndPort(std::string hostandport)
 */
int CIPv4Address::seperateHostAndPort(const std::string& hostandport, std::string* host, unsigned short* port)
{
  int pos = hostandport.find(":", 0);
  if(pos != 0) {
    // found ':' at pos
    cerr << "deviding " << hostandport << " into ";
    *port = strtoul(hostandport.substr(pos + 1, hostandport.size()).c_str(), NULL, 10);   
    *host = hostandport.substr(0, pos);
    cerr << "hostname " << *host << " and portnumber " << *port << endl;     
    return 0;
  }
  else {
    *port = 0;
    return -1;
  }
}



/*!
    \fn CIPv4Address::operator==(CIPv4Address)
 */
bool CIPv4Address::operator==( CIPv4Address& addr)
{
  struct sockaddr_in *other = addr.sock_addr_in_ptr();

  if( m_sockaddr_in.sin_family == other->sin_family && 
      m_sockaddr_in.sin_port == other->sin_port &&
      m_sockaddr_in.sin_addr.s_addr == other->sin_addr.s_addr) {
    return true;
  }
  else {
    return false;
  }
}


/*!
    \fn CIPv4Address::operator=(std::string hostname)
 */
void CIPv4Address::operator=(std::string hostname)
{
  string tmp_host;
  unsigned short tmp_port = 0;
  seperateHostAndPort(hostname, &tmp_host, &tmp_port);

  fillSockaddrIn(tmp_host, tmp_port);
    
}

ostream& operator<<(ostream& os, CIPv4Address addr) {
  
  os << addr.ipAddress() << ":" << addr.port();
  return os;
}
