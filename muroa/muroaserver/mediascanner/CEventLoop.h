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

#include <iostream>
#include <fstream>

class CMsgBase;

class CEventLoop {
public:
	CEventLoop(int sock_fd = 0);
	virtual ~CEventLoop();

	/** postEvent: put msg in internal event queue */
	void postEvent(CMsgBase *msg);
	/** sendEvent: send msg to external process */
	void sendEvent(CMsgBase *msg);
	int run();

protected:
	virtual bool handleMsg(CMsgBase* msg) = 0;
	void wakeUp();
	inline bool running() { return m_run; };
	void setSocket(int sockfd);

	std::ofstream m_dbg_file;

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
