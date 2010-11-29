/*
 * CNextlistCommand.cpp
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#include "CNextlistCommand.h"

CNextlistCommand::CNextlistCommand(QObject * parent) : CCommandBase(parent)
{
	m_cmdName = "modNextlist";
}

CNextlistCommand::CNextlistCommand(const QString& diff, QObject * parent): CCommandBase(parent)
{
	m_cmdName = "modNextlist";
	m_cmdData = diff;
}

CNextlistCommand::~CNextlistCommand()
{

}

void CNextlistCommand::setData(const QString& diff)
{
	m_cmdData = diff;
}

void CNextlistCommand::timeout()
{
	CCommandBase::timeout();
}
