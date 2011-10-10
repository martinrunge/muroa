/***************************************************************************
                          cmutex.cpp  -  description
                             -------------------
    begin                : Don M� 13 2003
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

#include "cmutex.h"

CMutex::CMutex(){
  pthread_mutexattr_init( &m_attributes );
  pthread_mutex_init( &m_mutex, &m_attributes );
  pthread_mutexattr_destroy( &m_attributes );
}
CMutex::~CMutex(){
  pthread_mutex_destroy( &m_mutex );
}

/** No descriptions */
void CMutex::Lock(void){
  pthread_mutex_lock( &m_mutex );
}

/** No descriptions */
void CMutex::UnLock(void){
  pthread_mutex_unlock( &m_mutex );
}

/** No descriptions */
bool CMutex::TryLock(void){
  return pthread_mutex_trylock( &m_mutex );
}
