/***************************************************************************
                          cmutex.h  -  description
                             -------------------
    begin                : Don Mär 13 2003
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

#ifndef CMUTEX_H
#define CMUTEX_H


/**
  *@author Martin Runge
  */

#include <pthread.h>


class CMutex {
public: 
	CMutex();
	~CMutex();

  /** No descriptions */
  bool TryLock(void);

  /** No descriptions */
  void UnLock(void);

  /** No descriptions */
  void Lock(void);
  
  inline pthread_mutex_t* GetMutexPtr() { return &m_mutex; };

private: // Private attributes
  /** the mutex itself */
  pthread_mutex_t m_mutex;

  /** mutex attributes */
  pthread_mutexattr_t m_attributes;

};

#endif
