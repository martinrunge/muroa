/***************************************************************************
                          cpthread.cpp  -  description
                             -------------------
    begin                : Die Feb 11 2003
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

#include "cpthread.h"

#include "cthreadslave.h"

#include <iostream>

using namespace std;

CPThread::CPThread(CThreadSlave* thread_slave) {
    m_run_thread = false;
    m_is_running = false;
    m_thread_slave = thread_slave;
}

CPThread::~CPThread() {
    StopThread();
}


int CPThread::StartThread(void) {

    int retval;
    Run(true);
    retval = pthread_create(&m_thread_id, NULL, (start_routine_ptr)CPThread::ThreadMainLoop, (void*) this);
    return retval;
}

int CPThread::StopThread() {
    int retval;

    Run(false);
    retval = pthread_join(m_thread_id, NULL);
    perror("CPThread::StopThread (pthread_join)");
    return retval;
}

int CPThread::ThreadMainLoop(CPThread* pthread_object) {
    int retval = 0;
    pthread_object->IsRunning(true);
    while (pthread_object->Run()) {
        pthread_object->ThreadSlave()->DoLoop();
    }
    pthread_object->IsRunning(false);
    return retval;
}

