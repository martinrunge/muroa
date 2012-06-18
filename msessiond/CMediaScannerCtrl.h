/*
 * CMediaScannerCtrl.h
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNERCTRL_H_
#define CMEDIASCANNERCTRL_H_



#include "CSubProcess.h"
#include "../mmscanner/CEventLoop.h"
#include "../mmscanner/CMsgQuit.h"
#include "../mmscanner/CMsgResponse.h"
#include "../mmscanner/CMsgProgress.h"
#include "../mmscanner/CMsgFinished.h"
#include "../mmscanner/CMsgCollectionChanged.h"


#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <signal.h>
#include <thread>
#include <mutex>
#include <condition_variable>

class ExInvMsg;

namespace muroa {

class CSession;


class CMediaScannerCtrl : public CSubProcess {
public:
	CMediaScannerCtrl(muroa::CSession *parent, boost::asio::io_service& io_service);
	virtual ~CMediaScannerCtrl();

	void start();
	void stop();
	void terminate();

    void sendMsg(CMsgBase* msg);

	void fromStdout(const char* buffer, int length);
	void fromStderr(const char* buffer, int length);


private:
	bool handleMsg(CMsgBase* msg);


//    std::mutex m_mutex;
//    std::condition_variable m_cond_var;
	uint32_t m_currentJobID;
	boost::asio::local::stream_protocol::socket m_socket;

    /// Buffer for incoming data.
	boost::array<char, 8192> m_buffer;

	char* m_bufPtr;
	int m_bufSize;
	int m_bufCapacity;

	muroa::CSession *m_parent;
};
}
#endif /* CMEDIASCANNERCTRL_H_ */
