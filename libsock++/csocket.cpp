/***************************************************************************
                          csocket.cpp  -  description
                             -------------------
    begin                : Don Jan 2 2003
    copyright            : (C) 2003 by Martin Runge
    email                : martin.runge@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "csocket.h"
#include <sys/socket.h>

#include <fcntl.h>

#include <iostream>

using namespace std;

CSocket::CSocket(__socket_type type, unsigned short port){
  m_timeout = -1;  //blocking
  m_is_bound = false;
  m_is_connected = false;
  m_port_nr = 0;
    
  m_connected_to_ip = 0;
  m_connected_to_port = 0;
  
  m_recv_from_addr = 0;

  m_last_hostname.clear();
 
  m_type = type;
  
  if(type == SOCK_STREAM)
    m_socket_descr = socket(PF_INET, SOCK_STREAM, 0);
  else
    m_socket_descr = socket(PF_INET, SOCK_DGRAM, 0);
 
  if(m_socket_descr == -1) { 
    perror("CSocket::CSocket could not create socket");
  }
    
  
  if(port != 0)
    bind(port);
  
    
}

CSocket::CSocket(int socket_descr, bool bound, bool connected){
  m_timeout = -1;  //blocking
  m_is_bound = bound;
  m_is_connected = connected;
  m_type = SOCK_STREAM;

  m_recv_from_addr = 0;  
  m_socket_descr = socket_descr;
}

CSocket::~CSocket(){
  if(m_socket_descr != -1) {
    close(m_socket_descr);
  }
}

/** bind to a port */
int CSocket::bind(unsigned short port){
  struct sockaddr_in sockaddr;

//   if(m_is_listening)
//     return -2;

  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(port);
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);


  int retval = ::bind(m_socket_descr, (struct sockaddr*) &sockaddr, sizeof(sockaddr));

  if(retval == 0) {
    m_port_nr = port;
    m_is_bound = true;
  }
  else {
    m_port_nr = 0;
    m_is_bound = false;
    perror("CSocket::bind");
  }
  return retval;
}


/** connect to a server */
int CSocket::connect(const unsigned long ip_addr, const unsigned short port){
  struct in_addr addr;
  addr.s_addr = htonl(ip_addr);
  return do_connect(&addr, port);
}


/** connect to a server */
int CSocket::connect(const std::string hostname, const unsigned short port){

  unsigned short tmp_port;
  std::string tmp_host;
  
  struct hostent *hostinfo;

  if(port == 0) {
    int retval = seperateHostAndPort(hostname, &tmp_host, &tmp_port);    
    if(retval == -1) {
      // cannot connect without port given
      return -1; 
    }
  }
  else {
    tmp_port = port;
    tmp_host = hostname;  
  }
  
  hostinfo = gethostbyname (tmp_host.c_str());
  if (hostinfo == NULL)
    {
      fprintf (stderr, "Unknown host %s.\n", hostname.c_str());
      return -1;
    }
  return do_connect((struct in_addr*)hostinfo->h_addr, tmp_port);  
    
}

int CSocket::do_connect(const struct in_addr* sock_addr_in, const unsigned short port) {
  struct sockaddr_in servername;

  servername.sin_family = AF_INET;
  servername.sin_port = htons (port);
  servername.sin_addr = *(struct in_addr *) sock_addr_in;

  if(::connect( m_socket_descr, (struct sockaddr*) &servername, sizeof(servername)) < 0){
    perror("connect");
    return -1;
  }
  m_is_connected = true;
  connectedToPort(port);
  connectedToAddress(servername.sin_addr.s_addr);
  return 0;

}


/** read from the socket connection */
int CSocket::read(char* buffer, int bufferlen){
	struct timeval timeout;
  int retval;
  
  if(m_type == SOCK_STREAM) {
    if(m_is_connected == false) {
      cerr << "Error: TCP socket must be connected for reading." << endl;
      return -1;
    }
  }
  else {
    if(m_is_bound == false) {
      cerr << "Error: UDP socket must be bound for reading." << endl;
      return -1;
    }
  }
    
  bool can_read = false;

	fd_set  fd_read_set, fd_write_set, fd_exept_set;

	timeout = m_timeout_timeval;

	FD_ZERO(&fd_read_set);
	FD_ZERO(&fd_exept_set);
  FD_ZERO(&fd_write_set);

  FD_SET(m_socket_descr, &fd_read_set);           
  FD_SET(m_socket_descr, &fd_exept_set);

	retval = select(m_socket_descr + 1, &fd_read_set, &fd_write_set, &fd_exept_set, &timeout);
	if(retval >= 1) {
  	if(FD_ISSET(m_socket_descr, &fd_read_set)) {
      // perror("CSocket::Read  can read: ");
      can_read = true;
		}

  	if(FD_ISSET(m_socket_descr, &fd_exept_set)) {
    	cout << "exception occoured" << endl;
		}
	}
	else {
		if(retval == 0) {
      // cerr << ".";
    	// timeout while waiting for data
      return 0;
		}
		else {  // retval < 0
      perror("select returned a value < 0");
      cerr  << "m_socket_descr = " << m_socket_descr << endl;
    	if(errno == EINTR) {
				cerr << "interrupted." << endl;
				return 0;
		  }
	  }
  }		

  struct sockaddr* sockaddr_ptr = reinterpret_cast<struct sockaddr*>(m_recv_from_addr->sock_addr_in_ptr());
  socklen_t len;

  retval = ::recvfrom(m_socket_descr, buffer, bufferlen, 0, sockaddr_ptr, &len);

  if(retval == 0 && can_read) {
    cerr << " the other side closed the connection." << endl;
    return -2;
  }
  // cerr << "recv returned " << retval << endl;
  if( retval > 0) return retval;

  if(errno == EINTR) {
    cerr << "Connection closed....." << endl;
  }
  else {
    cerr << "CSocket::Read - Error. recv returned negatice value, errno == " << errno << endl;
  }
  if(len != sizeof(struct sockaddr_in)) {
    cerr << "CSocket::Read - recvfrom return size " << len << " for sender address. sizeof(struct sockaddr_in)=" << sizeof(struct sockaddr_in) << " !" << endl;
  }
  else {
    // m_recv_from_addr seems ok
  }
  return retval;

}

/** write data to the socket */
int CSocket::write(const char* buffer, int bufferlen){
  int retval;
  
  if(m_type == SOCK_STREAM) {
    if(m_is_connected == false) {
      cerr << "Error: TCP socket must be connected for writing." << endl;
      return -1;
    }
  }
  else {
    if(m_is_connected == false) {
      cerr << "Error: UDP socket must be connected for writing without giving receiver address." << endl;
      return -1;
    }
  }

  
  retval = ::send(m_socket_descr, buffer, bufferlen, 0);
  if(retval != bufferlen) {
    cerr << "CSocket::Write: not the whole buffer was written!" << endl;
    if(retval < 0) {
      perror("CSocket::Write returned a negative value");
    }
  }
  return retval;
}

/** set the socket descriptor */
void CSocket::socketDescr(int socket){
  m_socket_descr = socket;
}

/** get the socket descriptor */
int CSocket::socketDescr(void){
  return m_socket_descr;
}

/** set the socket to non blocking mode. A timeout can be specified. If 0, a read call returns immediately, a positive number specifies the timeout in ms.
 ATTETION:  Non blocking mode cannot be disabled any more.*/
int CSocket::setNonBlocking(int timeout){
  int retval;

	m_timeout = timeout;
	m_timeout_timeval.tv_sec = timeout / 1000;
	m_timeout_timeval.tv_usec = timeout - (m_timeout_timeval.tv_sec * 1000);

  retval = fcntl(m_socket_descr, O_NONBLOCK);
  return 0;
}


/*!
    \fn CSocket::sendTo(char* data, int len, std::string hostname, unsigned short port = 0)
 */
int CSocket::sendTo(const char* data, const int bufferlen, const std::string hostname, const unsigned short port)
{
  unsigned short tmp_port;
  std::string tmp_host;
  
  struct hostent* hostinfo; 
  m_receiver.sin_family = AF_INET;
  
  tmp_port = port;
  tmp_host = hostname;
  
  if(hostname.compare(m_last_hostname) != 0) {
    // hostname differs from the one used in the last call to sendTo  
    // fill in the
    if(tmp_port == 0) {
      int retval = seperateHostAndPort(hostname, &tmp_host, &tmp_port);    
      if(retval == -1) {
        // cannot connect without port given
        return -1;
      } 
    }
    
    hostinfo = gethostbyname (tmp_host.c_str());
    if (hostinfo == NULL) {
      fprintf (stderr, "Unknown host %s.\n", tmp_host.c_str());
      return -2;
    }
    m_receiver.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    m_last_hostname = hostname;
  }
                           
  m_receiver.sin_port = htons (tmp_port);

  
  int retval = ::sendto(m_socket_descr, data, bufferlen, 0, (struct sockaddr*) &m_receiver, sizeof(m_receiver));
  if(retval != bufferlen) {
    cerr << "CSocket::Write: not the whole buffer was written!" << endl;
    if(retval < 0) {
      perror("CSocket::Write returned a negative value");
    }
  }
  return retval;
}


/*!
    \fn CSocket::seperateHostAndPort(std::string hostandport)
 */
int CSocket::seperateHostAndPort(const std::string& hostandport, std::string* host, unsigned short* port)
{
  int pos = hostandport.find(":", 0);
  if(pos != 0) {
    // found ':' at pos
    cerr << "deviding " << hostandport << " into ";
    *port = strtoul(hostandport.substr(pos + 1, hostandport.size()).c_str(), NULL, 10);   
    *host = hostandport.substr(0, pos);
    cerr << "hostname " << *host << " and portnumber " << *port << endl;     
  }
  
  if(*port == 0)
    return -1;
  else
    return 0;
}


unsigned long CSocket::connectedToAddress(){
  return m_connected_to_ip;
}

void CSocket::connectedToAddress(unsigned long ip_addr){
  m_connected_to_ip = ip_addr;
}

                        
unsigned short CSocket::connectedToPort() {
  return m_connected_to_port;
}
                        
void CSocket::connectedToPort(unsigned short port) {
  m_connected_to_port = port;
}


/*!
    \fn CSocket::recordSenderWithRecv(bool record)
 */
void CSocket::recordSenderWithRecv(bool record) {
    if(record == true) {
      if(m_recv_from_addr == 0) {
        m_recv_from_addr = new CIPv4Address();        
      }
    }
    else {
      if(m_recv_from_addr != 0) {
        delete m_recv_from_addr;
        m_recv_from_addr = 0;
      }
    }   
}


/*!
    \fn CSocket::recordSenderWithRecv(bool record)
 */
bool CSocket::recordSenderWithRecv(void) {
  if(m_recv_from_addr == 0)
    return false;
  else
    return true;
}

CIPv4Address* CSocket::latestSender() {
  return m_recv_from_addr;
}
