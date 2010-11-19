/*
 * CEventLoop.h
 *
 *  Created on: 16 Nov 2010
 *      Author: martin
 */

#ifndef CEVENTLOOP_H_
#define CEVENTLOOP_H_

#include <thread>
#include <queue>

class CMsgBase;

class CEventLoop {
public:
	CEventLoop(int sock_fd = 0);
	virtual ~CEventLoop();

	void postEvent(CMsgBase *msg);
	int run();

protected:
	virtual int handleMsg(CMsgBase* msg) = 0;
	void wakeUp();

private:
	void preparePollFDs();

	bool readable(int fd);

	struct pollfd *m_poll_fds;
	int m_num_poll_fds;

	int m_socket;
	int m_notify_fds[2];
	bool m_run;

    pthread_t m_mainThreadID;
    std::queue<CMsgBase*> m_eventQueue;
    std::mutex m_eventQueueMutex;
};

#endif /* CEVENTLOOP_H_ */
