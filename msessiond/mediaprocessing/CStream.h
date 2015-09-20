/*
 * CStream.h
 *
 *  Created on: 13 Jun 2010
 *      Author: Martin Runge
 */

#ifndef CSTREAM_H_
#define CSTREAM_H_

#include "avahi/CServiceDesc.h"
#include "boost/asio.hpp"
#include "CDecoder.h"

namespace std {
  class thread;
}

class IAudioIO;

namespace muroa {
  class CSession;
  class CMediaItem;
}

class CStreamServer;

enum sessionState {
	e_stopped = 0,
	e_playing = 1,
	e_paused = 2,
	e_none,
};


class CStream  {
public:
	CStream(muroa::CSession* session, boost::asio::io_service& io_service, int timeServicePort = 0);
	virtual ~CStream();

	void next() const;
	void setProgress( int playedSecs, int totalSecs ) const ;

	// TODO: get rid of write in the public API here.
	int write(char* data, int size)  const;

	void finished() const;
	void progress(int done, int total) const;

	void play();
	void pause();
	void stop();

	void operator()();

	void addReceiver(muroa::ServDescPtr srv_desc_ptr);
	void rmReceiver(const std::string& name);

	void adjustReceiverList(std::vector<muroa::ServDescPtr> receivers);

private:
 	void startThread();
 	void stopThread();

	enum sessionState m_state;
	std::string m_fileName;

	int m_done;
	int m_total;

 	CDecoder m_decoder;
 	IAudioIO* m_audioIO;

	muroa::CSession* m_session;

 	std::thread* m_thread;
	bool m_run;

    CStreamServer *m_streamserver;
    boost::asio::io_service& m_io_service;
};

#endif /* CSTREAM_H_ */
