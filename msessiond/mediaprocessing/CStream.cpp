/*
 * CStream.cpp
 *
 *  Created on: 13 Jun 2010
 *      Author: martin
 */

#include <thread>

#include "CStream.h"

#include <CMediaItem.h>
#include "IAudioIO.h"
#include "CAudioIOAlsa.h"
#include "CSession.h"

#include <cmds/CmdProgress.h>
#include <cmds/CmdFinished.h>

#include <log4cplus/logger.h>
#include "CApp.h"

using namespace log4cplus;
using namespace muroa;
using namespace std;

CStream::CStream(CSession* session) : m_done(0), m_state(e_stopped), m_decoder(this), m_session(session), m_thread(0), m_run(false)
{
    m_audioIO = new CAudioIoAlsa();
}

CStream::~CStream() {
	stop();
	delete m_audioIO;
}

int CStream::write(char* data, int size) const
{
	int amount = m_audioIO->write(data, size);
	return amount;
}

void CStream::setProgress(  int playedSecs, int totalSecs ) const {
	CmdProgress* progCmd = new CmdProgress();
	m_session->postIncomingCmd(progCmd);
}

void CStream::operator()()
{
	while(m_run)
	{
		int sbsize = m_decoder.decode();
		if ( sbsize == -1 ) {
			// end if stream
			m_decoder.close();

			// next song here or leave thread loop and reopen completely for next song?

		    CMediaItem *item = m_session->getCurrentMediaItem();
			m_decoder.open(item->getFilename());
		}
	}
}

void CStream::startThread()
{
	if(m_thread == 0 && m_run == false)
	{
		m_run = true;
		m_thread = new std::thread(bind(&CStream::operator(), this)  );
	}
}

void CStream::stopThread()
{
	if(m_thread != 0)
	{
		m_run = false;
		m_thread->join();
		delete m_thread;
		m_thread = 0;
	}
}


void CStream::play()
{
	LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::play");
	switch(m_state)
	{
	case e_stopped:
	{
		assert(!m_decoder.isOpen());
		assert(m_thread == 0);
		assert(m_run == false);

	    m_audioIO->open("default" , 44100, 2);

	    CMediaItem *item = m_session->getCurrentMediaItem();
		m_decoder.open(item->getFilename());
		startThread();
		m_state = e_playing;
		break;
	}
	case e_paused:

		m_state = e_playing;
		break;

	case e_playing:
		LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::play: already in playing state");
		break;
	case e_none:
	default:
		LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::play: unknown state!");
		break;
	}
}

void CStream::pause()
{
	LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::pause");

}


void CStream::stop()
{
	LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::stop");
	switch(m_state)
	{
	case e_playing:
	case e_paused:
	{
		assert(m_thread != 0);
		assert(m_run == true);

		stopThread();
		m_decoder.close();
		m_state = e_stopped;
		break;
	}
	case e_stopped:
		LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::play: already in state stopped");
		break;
	case e_none:
	default:
		LOG4CPLUS_DEBUG(CApp::getInstPtr()->logger(), "CStream::play: unknown state!");
		break;
	}

	m_audioIO->close();
	m_done = 0;
	CmdProgress* progCmd = new CmdProgress(m_done, m_total);
	m_session->postIncomingCmd(progCmd);
}


void CStream::next() const
{
	CmdFinished* cmdFini = new CmdFinished();
	m_session->postIncomingCmd(cmdFini);
}

