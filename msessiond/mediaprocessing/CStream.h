/*
 * CStream.h
 *
 *  Created on: 13 Jun 2010
 *      Author: martin
 */

#ifndef CSTREAM_H_
#define CSTREAM_H_

#include "CDecoder.h"

namespace std {
  class thread;
}

class IAudioIO;

namespace muroa {
  class CSession;
  // class CMediaItem;
}

enum sessionState {
	e_stopped = 0,
	e_playing = 1,
	e_paused = 2,
	e_none,
};


class CStream  {
public:
	CStream(muroa::CSession* session);
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
};

#endif /* CSTREAM_H_ */
