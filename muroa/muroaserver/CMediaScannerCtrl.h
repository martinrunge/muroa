/*
 * CMediaScannerCtrl.h
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNERCTRL_H_
#define CMEDIASCANNERCTRL_H_

#include "CSubProcess.h"
#include "mediascanner/CEventLoop.h"
#include "mediascanner/CMsgQuit.h"
#include "mediascanner/CMsgResponse.h"
#include "mediascanner/CMsgProgress.h"
#include "mediascanner/CMsgFinished.h"
#include "mediascanner/CMsgCollectionChanged.h"


#include <signal.h>
#include <thread>
#include <mutex>
#include <condition_variable>

class CMuroaServer;

class CMediaScannerCtrl: public CEventLoop {
public:
	CMediaScannerCtrl(CMuroaServer *parent);
	virtual ~CMediaScannerCtrl();

	void start();
	void stop();
	void terminate();


private:
	void waitPid();

	bool handleMsg(CMsgBase* msg);

	CSubProcess m_subProcess;
	pid_t m_pid;

	std::thread *m_thread;
	std::thread *m_waitthread;
	bool m_child_running;

    std::mutex m_mutex;
    std::condition_variable m_cond_var;

	struct sigaction m_old_action;

	CMuroaServer *m_parent;
};

#endif /* CMEDIASCANNERCTRL_H_ */
