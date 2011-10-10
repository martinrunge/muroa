/***************************************************************************
                          ctcpserversocket.h  -  description
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

#ifndef CTCPSERVERSOCKET_H
#define CTCPSERVERSOCKET_H


/**Implements a TCP server socket
  *@author Martin Runge
  */

#include "csocket.h"

class CTCPServerSocket : public CSocket {
public:
	CTCPServerSocket(unsigned short port = 0);
	~CTCPServerSocket();

  /** This function is blocking waiting for an incoming connection. Then it is returning
      the connected socket as CSocket. Should be called in a seperate thread loop to accept
      more than one connection. The returned CSocket* is allocated via new by accept and
      has to be deleted by the user after use via delete*/
  /** put to listening mode */
  int listen(void);
  CSocket* accept(void);

private: // Private attributes
  /** state machine */
  bool m_is_listening;


};

#endif
