/***************************************************************************
                          csocket.h  -  description
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

#ifndef CSOCKET_H
#define CSOCKET_H


/**A Socket that is connected
  *@author Martin Runge
  */

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include <string>

class CSocket {
public:
	CSocket(__socket_type type = SOCK_STREAM, unsigned short port = 0);
  
  CSocket(int sock_descr, const bool bound = false, const bool connected = false);

	~CSocket();
 
  /** bind to a port */
  int bind(const unsigned short port);
  
  /** connect to a server */
  int connect(const std::string hostname, const unsigned short port = 0);
  int connect(const unsigned long ip_addr, const unsigned short port);

  /** write data to the socket */
  int write(const char* buffer, const int bufferlen);
  /** read from the socket connection */
  int read(char* buffer, int bufferlen);
  /** set the socket to non blocjing mode. a timeout can be specified. If 0, a read call returns immediately, a positive number specifies the timeout in ms, and a negative number set the timeout to infinite, that is blocking. */
  int setNonBlocking(const int timeout);
  
  /** send data giving the address of the receiver */
  int sendTo(const char* data, const int bufferlen, const std::string hostname, const unsigned short port = 0);

    /*!
        \fn CSocket::getPort(void)
     */
  inline unsigned short getPort(void) { return m_port_nr; };
  
  
  unsigned long connectedToAddress(void);
  void connectedToAddress(unsigned long ip_addr);

  unsigned short connectedToPort();
  void connectedToPort(unsigned short port);

  

protected: // Protected methods
  /** get the socket descriptor */
  int socketDescr(void);
  void socketDescr(int socket);

  /*!
      \fn CSocket::isBound(void)
  */
  inline bool isBound(void){ return m_is_bound; };

  /** the timeout value in ms */
  long m_timeout;

  /** needed for select. The timeout value devided in seconds an microseconds */
  struct timeval m_timeout_timeval;

  /** The socket descriptor */
  int m_socket_descr;

private: // Private attributes

  int do_connect(const struct in_addr* sock_addr_in, const unsigned short port);


  /** indicates wether the socket is already bound or not */  
  bool m_is_bound;
  
  /** indicated wether the socket is connected */
  bool m_is_connected;

  /** port number */
  unsigned short m_port_nr;
  
  /** keep the hostname from the last call of sendTo. 
      Used to improve performance by caching struct sockaddr_in generated 
      the last time with the same hostname. Port is not neccessary to cache, because
      it is much less expensive to fill into struct sockaddr (esp. no DNS lookup) */
 
  std::string m_last_hostname;
  struct sockaddr_in m_receiver;
  int m_type;
  
  /** if socket is of type SOCK_STREAM and it is connected, these variables may hold the IP address and port of the 
      socket on the other end. It is filled by accept an connect*/
  unsigned long m_connected_to_ip;
  unsigned short m_connected_to_port;
                 
    
  int seperateHostAndPort(const std::string& hostandport, std::string* host, unsigned short* port);
  
};

#endif
