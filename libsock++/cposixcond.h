/***************************************************************************
 *   Copyright (C) 2003 by Martin Runge                                    *
 *   martin.runge@web.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CPOSIXCOND_H
#define CPOSIXCOND_H

/**
C++ wrapper for posix conditional variables

@author Martin Runge
*/

#include <pthread.h>

class CMutex;

class CPosixCond{
public:
  CPosixCond(CMutex* mutex = 0);

  ~CPosixCond();

  int Signal(void);

  int Broadcast(void);

  int Wait(void);

  int Wait(int timeout_in_ms);

private:
  pthread_cond_t m_cond_var;

  CMutex* m_mutex;

  bool m_own_mutex;
};

#endif
