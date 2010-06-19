/*
 * CStream.cpp
 *
 *  Created on: 13 Jun 2010
 *      Author: martin
 */

#include "CStream.h"
#include <QTimer>

#include <QDebug>

#include "CCollectionItem.h"

CStream::CStream(): m_done(0), m_state(e_stopped) {
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

CStream::~CStream() {
	stop();
	delete m_timer;
}


void CStream::setSong( CCollectionItem* item )
{
	m_fileName = item->getFilename();
	m_total = item->getLengthInSec();
}

void CStream::play()
{
	qDebug() << QString("CStream::play");
	switch(m_state)
	{
	case e_stopped:

	    m_timer->start(1000);
		m_state = e_playing;
		break;

	case e_paused:

		m_timer->start(1000);
		m_state = e_playing;
		break;

	case e_playing:

	    m_timer->stop();
		m_state = e_paused;
		break;

	case e_none:
	default:
		qDebug() << QString("CStream::play: unknown state!");
		break;
	}
}

void CStream::stop()
{
    m_timer->stop();
	m_state = e_stopped;
	m_done = 0;
	emit progress(m_done, m_total );
}



void CStream::timeout()
{
	m_done ++;
	emit progress(m_done, m_total);
	qDebug() << QString("timeout");
	if( m_done >= m_total )
	{
		emit finished();
	}
}
