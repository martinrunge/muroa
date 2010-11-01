/*
 * CMediaScanner.h
 *
 *  Created on: 23 Oct 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNER_H_
#define CMEDIASCANNER_H_

#include <iostream>
#include <fstream>

class CMsgBase;

class CMediaScanner {
public:
	CMediaScanner(int sock_fd);
	virtual ~CMediaScanner();

	int run();

private:
	int handleMsg(CMsgBase* msg);

	int m_socket;
	bool m_run;

    std::ofstream m_dbg_file;

};

#endif /* CMEDIASCANNER_H_ */
