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
#include <log4cplus/loggingmacros.h>
#include "CApp.h"

#include "CStreamServer.h"

using namespace log4cplus;
using namespace muroa;
using namespace std;

CStream::CStream(CSession* session, boost::asio::io_service& io_service, int timeServicePort) : m_done(0),
		                                                   m_state(e_stopped),
		                                                   m_decoder(this),
		                                                   m_session(session),
		                                                   m_thread(0),
		                                                   m_run(false),
														   m_io_service(io_service)
{
    m_audioIO = new CAudioIoAlsa();
    m_streamserver = new CStreamServer(io_service, m_session->getName(), timeServicePort);
}

CStream::~CStream() {
	stop();
	delete m_audioIO;
	delete m_streamserver;
}

int CStream::write(char* data, int size) const
{
    int amount = m_streamserver->write(data, size);
//	int amount = m_audioIO->write(data, size);
	return amount;
}

void CStream::setProgress(  int playedSecs, int totalSecs ) const {
	CmdProgress* progCmd = new CmdProgress();
	progCmd->setProgress(playedSecs, totalSecs);

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
			m_session->addNextlistRevFromNextCmd();

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
		m_streamserver->open();
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
		m_streamserver->flush();
        m_streamserver->close();
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
	m_streamserver->flush();
	m_streamserver->open();

	CmdFinished* cmdFini = new CmdFinished();
	m_session->postIncomingCmd(cmdFini);

	CmdProgress* progCmd = new CmdProgress(m_done, m_total);
	m_session->postIncomingCmd(progCmd);
}


void CStream::addReceiver(ServDescPtr srv_desc_ptr)
{
    // CIPv4Address addr(srv_desc_ptr->getHostName(), srv_desc_ptr->getPortNr());
    bip::tcp::endpoint endp(srv_desc_ptr->getAddress(), srv_desc_ptr->getPortNr());
    m_streamserver->addClient(endp, srv_desc_ptr->getServiceName());
}

void CStream::rmReceiver(const string& name)
{
    m_streamserver->removeClient(name);
}

void CStream::adjustReceiverList(vector<ServDescPtr> receivers)
{
    CRootItem* ri = m_session->getSessionState();
    CCategoryItem* base = ri->getCategoryPtr("/RenderClients");

    for(int i = 0; i < base->getNumContentItems(); i++)
    {
        IContentItem* ci = base->getContentItem(i);
        if(ci->type() == CItemType::E_STREAM_CLIENT)
        {
            CStreamClientItem *sci= reinterpret_cast<CStreamClientItem*>(ci);
            ///TODO make these two lists equal

        }


    }


}

