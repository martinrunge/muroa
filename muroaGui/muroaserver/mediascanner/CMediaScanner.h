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

#include <queue>
#include <mutex>

#include <pthread.h>

#include "CEventLoop.h"

class CMsgBase;

class CMediaScanner : public CEventLoop {
public:
	CMediaScanner( int sock_fd = 0 );
	virtual ~CMediaScanner();

private:
	int handleMsg(CMsgBase* msg);

	std::ofstream m_dbg_file;
};

#endif /* CMEDIASCANNER_H_ */
