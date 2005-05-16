/***************************************************************************
                          ctcpserversocket.cpp  -  description
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "ctcpserversocket.h"
#include "csocket.h"

using namespace std;

extern bool global_termination_flag;


CTCPServerSocket::CTCPServerSocket(unsigned short port) : CSocket(SOCK_STREAM, port) {
  
  m_is_listening = false;
  

  if( socketDescr() < 0) {
    // error
  }

  if(isBound()) {
    listen();
  }

}


CTCPServerSocket::~CTCPServerSocket(){

}

/** This function is blocking waiting for an incoming connection. Then it is returning the connected socket as CSocket. Should be called in a seperate thread loop to accept more than one connection. */
CSocket* CTCPServerSocket::accept(void){

  struct sockaddr_in clientaddr;
  socklen_t size;
  CSocket* connected_socket;

  struct timeval timeout_timeval, *timeval_ptr;
  fd_set  fd_read_set, fd_write_set, fd_exept_set;

  int select_retval, accept_retval;
  bool can_accept = false;

  if(!m_is_listening)
    return 0;

  if(m_timeout < 0) {

    connected_socket = new CSocket(::accept( socketDescr(), (struct sockaddr*) &clientaddr, &size), false, true);
    // the new socket is not bound, but connected. Portnumber is provided by the system.
    connected_socket->connectedToPort(ntohs(clientaddr.sin_port));
    connected_socket->connectedToAddress(ntohl(clientaddr.sin_addr.s_addr));
    
    return connected_socket;
  }
  else {
    do {
      if(m_timeout == 0) {
        timeval_ptr = NULL;
      }
      else { // m_timeout > 0
        timeout_timeval.tv_sec = m_timeout / 1000;
        timeout_timeval.tv_usec = m_timeout - (timeout_timeval.tv_sec * 1000);
        timeval_ptr = &timeout_timeval;
      }

      FD_ZERO(&fd_read_set);
      FD_ZERO(&fd_exept_set);

      FD_SET(m_socket_descr, &fd_read_set);
      FD_SET(m_socket_descr, &fd_exept_set);

      select_retval = ::select(socketDescr() + 1, &fd_read_set, NULL, &fd_exept_set, timeval_ptr);
      if(select_retval == 1) {
        if(FD_ISSET(socketDescr(), &fd_read_set)) {
          cout << "can accept ... ";
          can_accept = true;
          accept_retval = ::accept(socketDescr(), (struct sockaddr*) &clientaddr, &size);
          if(select_retval >= 0) {
            connected_socket = new CSocket(accept_retval, false, true);
            connected_socket->connectedToAddress(ntohl(clientaddr.sin_addr.s_addr));
            connected_socket->connectedToPort(ntohs(clientaddr.sin_port));
            // new socket is not bound, but connected.
            return connected_socket;
          }
          else { // accept returned a negative value, probably -1
            if(errno == EINTR) {
              cerr << "interrupted by a signal ..." << endl;
              // return -1;
            }
          }
        }

        if(FD_ISSET(m_socket_descr, &fd_exept_set)) {
          cout << "exception occoured" << endl;
        }
      }
      else {
        if(select_retval == 0) {
          cerr << ".";
          // timeout while waiting for data
          // return 0;
        }
        else if(select_retval < 0) {  // retval < 0
          cerr << "select returned a value < 0: errno: " << errno << endl;
          if(errno == EINTR) {
            cerr << "interrupted by a signal ..." << endl;
            // return -1;
          }
        }
      }
    }
    while(global_termination_flag == false);
  }

  return 0;
  // build in a cancellation point to make it possible to exit the accept thread nicely when server is shut down.

}



/** put to listening mode */
int CTCPServerSocket::listen(void){

  if(isBound() == false)
    return -1;


  int retval = ::listen(socketDescr(), 1);
  if(retval == 0)
    m_is_listening = true;
  else
    m_is_listening = false;    

  return retval;

}

