/*
 * CEventLoop.cpp
 *
 *  Created on: 16 Nov 2010
 *      Author: martin
 */

#include "CEventLoop.h"
#include "CMsgBase.h"

#include "../Exceptions.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <poll.h>
#include <string.h>

using namespace std;

CEventLoop::CEventLoop(int sock_fd) : m_poll_fds(0),
                                      m_num_poll_fds(0),
                                      m_socket(sock_fd),
                                      m_run(false),
                                      m_mainThreadID( pthread_self() ) {

	int rc = socketpair(AF_UNIX, SOCK_STREAM, 0, m_notify_fds);
	preparePollFDs();
}

CEventLoop::~CEventLoop() {

}

void CEventLoop::setSocket(int sockfd) {
	m_socket = sockfd;
	preparePollFDs();
}


void CEventLoop::postEvent(CMsgBase *msg) {
	{
		std::lock_guard<std::mutex> lk(m_eventQueueMutex);
		m_eventQueue.push(msg);
		wakeUp();
	}
	// pthread_kill(m_mainThreadID, SIGUSR1);
}

void CEventLoop::sendEvent(CMsgBase *msg) {
	ssize_t written;
	int size;
	const char * buffer = msg->serialize(size);

	written = write(m_socket, buffer, size);

}


int CEventLoop::run() {
	CMsgBase* msg;
	int rc = 0;
	m_run=true;
	int runs = 0;

	const int totalBufferSize = 20 * 256 + 1;
	char buffer[totalBufferSize];
	char* bufPtr = buffer;                   // bufPtr -> beginning of unused data in buffer
	int bufferSize = 0;                      // size of received data
	char* bufFreePtr = buffer;               // bufFreePtr -> beginning of free buffer space
	int bufferFreeSize = totalBufferSize;     // remaining part, beginning at bufFreePtr.

	while(m_run) {
		// wait for a CMsg object to arrive on socket or for one that has been posted to eventQueue via postEvent()

		int pollrc = poll( m_poll_fds, m_num_poll_fds, -1 );

		m_dbg_file << "inner loop" << endl;
		if(readable(m_notify_fds[1])) {
			m_dbg_file << "m_notify_fd readable" << endl;
			uint8_t notify_byte;
			read(m_notify_fds[1], &notify_byte, sizeof(uint8_t));

			// see, if there is an event in eventQueue, continue waiting in recv() otherwise.
			{
				std::lock_guard<std::mutex> lk(m_eventQueueMutex);
				if(m_eventQueue.size() > 0) {
					msg = m_eventQueue.front();
					m_eventQueue.pop();
				}
			}
			m_run = handleMsg(msg);
		}
		else {
			if(readable(m_socket)) {
				m_dbg_file << "m_socket readable" << endl;
				ssize_t numBytes = recv(m_socket, bufFreePtr, bufferFreeSize, 0);
				m_dbg_file << "recv: " << numBytes << " bytes of data" << endl;
				bufferSize += numBytes;
				bufferFreeSize -= numBytes;
				bufFreePtr += numBytes;
			}
			while(true) {
				try {
					m_dbg_file << "calling msgFactory: bufPtr=" << hex << (void*)bufPtr << dec << " bufferSize=" << bufferSize << endl;
					msg = CMsgBase::msgFactory( bufPtr, bufferSize );
					m_dbg_file << "after return: bufPtr=" << hex << (void*)bufPtr << dec << " bufferSize=" << bufferSize << endl;
				}
				catch(NeedMoreDataException dataEx) {
					m_dbg_file << dataEx.reason() << endl;
					// cleanup ringbuffer
					int freeSpaceAtBeginning = bufPtr - buffer;
					if(freeSpaceAtBeginning > bufferSize) {
						memcpy(buffer, bufPtr, bufferSize);
					}
					else {  // overlapping, should be avoided
						memmove(buffer, bufPtr, bufferSize);
					}
					bufPtr = buffer;
					bufFreePtr = bufPtr + bufferSize;
					bufferFreeSize = totalBufferSize - bufferSize;

					break;
				}

				// handle the CMsg object, no matter if it was receiver via socket or eventQueue.
				m_run = handleMsg(msg);
				if(m_run == false) break;
			}

		}
	}
	return rc;
}

void CEventLoop::wakeUp() {
	uint8_t notifier_byte = 1;
	write(m_notify_fds[0], &notifier_byte, sizeof(uint8_t));
}


void CEventLoop::preparePollFDs() {
	if(m_poll_fds != 0) {
		delete [] m_poll_fds;
		m_poll_fds = 0;
	}

	m_num_poll_fds = 1;    // m_notify_fds[1] is always present !
	if(m_socket != 0) {
		m_num_poll_fds++;
	}

	m_poll_fds = new struct pollfd[m_num_poll_fds];

	m_poll_fds[0].fd = m_notify_fds[1];
	m_poll_fds[0].events = POLLIN;

	if( m_num_poll_fds > 1) {
		m_poll_fds[1].fd = m_socket;
		m_poll_fds[1].events = POLLIN;
	}
}

bool CEventLoop::readable(int fd) {
	bool readable = false;
	for(int i = 0; i < m_num_poll_fds; i++) {
		if( m_poll_fds[i].revents & POLLIN && m_poll_fds[i].fd == fd) {
			readable = true;
		}
	}
	return readable;
}
