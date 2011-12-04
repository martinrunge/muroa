/*
 * CCommand.cpp
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#include "CmdBase.h"

#include <QDebug>

int CmdBase::m_timeout_in_ms = 100;
unsigned CmdBase::m_first_free_id = 0;

using namespace std;

CmdBase::CmdBase(CmdBase::Type cmdType, int knownRev, QObject * parent) : m_known_rev(knownRev), m_type(cmdType), QObject(parent) {

	m_timer.setSingleShot(true);
	m_timer.setInterval(m_timeout_in_ms);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_Id = m_first_free_id;
	m_first_free_id++;
}

CmdBase::~CmdBase() {

}

void CmdBase::send()
{
	m_timer.start();
}

void CmdBase::ack()
{
	m_timer.stop();
}

void CmdBase::timeout()
{
	qDebug() << QString("Command %1 timed out after %2 ms.").arg(QString::fromUtf8(name().c_str())).arg(m_timeout_in_ms);
}
