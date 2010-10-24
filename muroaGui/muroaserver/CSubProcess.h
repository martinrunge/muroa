/*
 * CSubProcess.h
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#ifndef CSUBPROCESS_H_
#define CSUBPROCESS_H_

#include <vector>
#include <string>

class CSubProcess {
public:
	CSubProcess();
	virtual ~CSubProcess();

	int start(std::string executable, std::vector<std::string> args, int cpu_prio, int io_prio);
	int waitForExit(int timeout_in_ms = 0);
	int sendSignal(int sig_no);
	inline int kill() { return sendSignal(9); }

	int read(char* buffer, int length);
	int write(char* buffer, int length);


private:
	int m_socket;
	int m_other_end;

};

#endif /* CSUBPROCESS_H_ */
