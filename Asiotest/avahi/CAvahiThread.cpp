/*
 * CAvahiThread.cpp
 *
 *  Created on: 11 Oct 2011
 *      Author: martin
 */

#include "CAvahiThread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <poll.h>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#include <iostream>

using namespace std;


CAvahiThread::CAvahiThread( boost::asio::io_service& io_service ) : m_io_service(io_service)
{

}


CAvahiThread::~CAvahiThread() {

}

void CAvahiThread::operator()() const {

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) perror("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5555;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
             perror("ERROR on binding");
    listen(sockfd,5);

    fprintf(stderr, "listening on 5555 ... \n");

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);

    if (newsockfd < 0) perror("ERROR on accept");

    try {

		setNonblocking(newsockfd);

		fprintf(stderr, "accepted connection on 5555 \n");

		struct pollfd fds[1];
		int timeout_msecs = 50000;
		int ret;
		int i;

		/* Open STREAMS device. */
		fds[0].fd = newsockfd;
		fds[0].events = POLLRDNORM;

		ret = poll(fds, 1, timeout_msecs);

		fprintf(stderr, "returned from poll\n");
		if(ret > 0) {
	//    	if(fds[0].revents & POLLOUT ) {
	//    		fprintf(stderr, "POLLOUT");
	//    	}
			if(fds[0].revents & POLLIN ) {
				fprintf(stderr, "POLLIN");
			}
			if(fds[0].revents & POLLHUP ) {
				fprintf(stderr, "POLLHUP");
			}
		}
		else {
			if(ret == 0) {
				fprintf(stderr, "TIMEOUT with poll");
			}
			else {
				perror("poll");
			}
		}

		bzero(buffer,256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) perror("ERROR reading from socket");
		fprintf(stderr, "Here is the message: %s\n",buffer);
    }
//    catch (abi::__forced_unwind&) {
//    	cerr << "cancel exception" << endl;
//    	throw;
//    }
    catch(...) {
    	cerr << "other exception" << endl;
    	throw;
    }
    close(newsockfd);
    close(sockfd);

}

int CAvahiThread::setNonblocking(int fd) const
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}

