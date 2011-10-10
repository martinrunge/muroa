/***************************************************************************
                          cpthread.h  -  description
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

#ifndef CPTHREAD_H
#define CPTHREAD_H

#include <pthread.h>
#include <errno.h>


// #include "cthreadslave.h"

class CThreadSlave;

typedef void* (*start_routine_ptr)(void*);

class CPThread  {

public:
    CPThread(CThreadSlave* thread_slave);
    ~CPThread(void);

    int StartThread(bool realtime = false);

    int StopThread(void);
    
    inline bool IsRunning(void) { return m_run_thread; };

    
protected:
    inline bool Run(void) { return m_run_thread; };


private:

    static int ThreadMainLoop(CPThread* pthread_object);


    inline void Run(bool run) { m_run_thread = run; };

    inline void IsRunning(bool run) { m_is_running = run; };


    inline void ThreadSlave(CThreadSlave* thread_slave) { m_thread_slave = thread_slave; };

		inline CThreadSlave* ThreadSlave(void) { return m_thread_slave; };

    pthread_t m_thread_id;
    pthread_attr_t m_thread_attr;

    bool m_run_thread;

    bool m_is_running;

    CThreadSlave* m_thread_slave;
};



#endif
