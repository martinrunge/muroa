/*
 * CStream.cpp
 *
 *  Created on: 13 Jun 2010
 *      Author: martin
 */

#include "CStream.h"
#include <QTimer>

#include <QDebug>

#include "../CCollectionItem.h"
#include "IAudioIO.h"
#include "CAudioIOAlsa.h"


CStream::CStream(): m_done(0), m_state(e_stopped), m_decoder(this) {
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
	emit progress(playedSecs, totalSecs);
}

void CStream::setSong( CCollectionItem* item )
{
	m_fileName = item->getFilename();
	m_total = item->getLengthInSec();

	qDebug() << QString("CStream::setSong %1").arg(m_fileName);
	start();
}

void CStream::start()
{
    m_audioIO->open("default" , 44100, 2);

    if(m_decoder.isOpen()) m_decoder.close();
	m_decoder.open(m_fileName.toUtf8());
}


void CStream::play()
{
	qDebug() << QString("CStream::play");
	switch(m_state)
	{
	case e_stopped:

		m_state = e_playing;
		break;

	case e_paused:

		m_state = e_playing;
		break;

	case e_playing:

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
	m_decoder.close();
    m_audioIO->close();

	m_state = e_stopped;
	m_done = 0;
	emit progress(m_done, m_total );
}


void CStream::next() const
{
	emit finished();
}

