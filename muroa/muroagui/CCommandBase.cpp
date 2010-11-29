/*
 * CCommand.cpp
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#include "CCommandBase.h"

#include <QDebug>

int CCommandBase::m_timeout_in_ms = 100;
int CCommandBase::m_first_free_id = 0;

CCommandBase::CCommandBase(QObject * parent) : QObject(parent) {

	m_timer.setSingleShot(true);
	m_timer.setInterval(m_timeout_in_ms);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_cmdId = m_first_free_id;
	m_first_free_id++;
}

CCommandBase::~CCommandBase() {

}

void CCommandBase::send()
{
	m_timer.start();
}

void CCommandBase::ack()
{
	m_timer.stop();
}

void CCommandBase::timeout()
{
	qDebug() << QString("Command %1 timed out after %2 ms.").arg(commandName()).arg(m_timeout_in_ms);
}
