/***************************************************************************
                          cthreadslave.h  -  description
                             -------------------
    begin                : Mit Feb 12 2003
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

#ifndef CTHREADSLAVE_H
#define CTHREADSLAVE_H


/**base class for all classes that contain an own thread. This is used to define an interface to the thread entrance point as pure virtual member.
  *@author Martin Runge
  */

class CThreadSlave {
public: 
	CThreadSlave();
	virtual ~CThreadSlave();
  /** To be overloaded by all classes that implement threaded operations. */
  virtual void DoLoop() = 0;
};

#endif
