/***************************************************************************
 *   Copyright (C) 2003 by Martin Runge                                    *
 *   martin.runge@web.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cposixcond.h"
#include "cmutex.h"
#include <time.h>

CPosixCond::CPosixCond(CMutex* mutex)
{
  if(mutex != 0) {
    m_mutex = mutex;
    m_own_mutex = false;
  }
  else {
    m_mutex = new CMutex;
    m_own_mutex = true;
  }

  // m_cond_var = PTHREAD_COND_INITIALIZER;

  pthread_cond_init(& m_cond_var, NULL);
}


CPosixCond::~CPosixCond()
{
  if(m_own_mutex)
    delete m_mutex;

  pthread_cond_destroy(&m_cond_var);
}


int CPosixCond::Signal(void) {
  return pthread_cond_signal(&m_cond_var);
}

int CPosixCond::Broadcast(void){
  return pthread_cond_broadcast(&m_cond_var);
}

int CPosixCond::Wait(void){
  int retval;
  if(m_own_mutex) {
    m_mutex->Lock();
  }
  retval = pthread_cond_wait(&m_cond_var, m_mutex->GetMutexPtr());
  if(m_own_mutex) {
    m_mutex->UnLock();
  }
  return retval;
}

int CPosixCond::Wait(int timeout_in_s) {
  int retval;

  struct timespec time_val;
  if(m_own_mutex) {
    m_mutex->Lock();
  }
  time_val.tv_sec = time(NULL);
  time_val.tv_sec += timeout_in_s;
  retval = pthread_cond_timedwait(&m_cond_var, m_mutex->GetMutexPtr(), &time_val );
  if(m_own_mutex) {
    m_mutex->UnLock();
  }
  return retval;
}

