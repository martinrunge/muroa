/*
 * CMediaScannerCtrl.h
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNERCTRL_H_
#define CMEDIASCANNERCTRL_H_

#include "CSubProcess.h"

#include <signal.h>
#include <thread>

class CMediaScannerCtrl {
public:
	CMediaScannerCtrl();
	virtual ~CMediaScannerCtrl();

	void start();
	void stop();

	void loop();
	void waitPid();

private:
	CSubProcess m_subProcess;
	int m_socket;
	pid_t m_pid;

	std::thread *m_thread;
	std::thread *m_waitthread;
	bool m_running;

	struct sigaction m_old_action;
};

#endif /* CMEDIASCANNERCTRL_H_ */
