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

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <signal.h>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace muroa {

class CSession;


class CMediaScannerCtrl : public CSubProcess {
public:
	CMediaScannerCtrl(muroa::CSession *parent, boost::asio::io_service& io_service);
	virtual ~CMediaScannerCtrl();

	void start(uint32_t jobID);
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
