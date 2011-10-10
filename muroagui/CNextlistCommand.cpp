/*
 * CNextlistCommand.cpp
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#include "CNextlistCommand.h"

CNextlistCommand::CNextlistCommand(int knownRev, QObject * parent) : CCommandBase(knownRev, parent)
{
	m_cmdName = "modNextlist";
}

CNextlistCommand::CNextlistCommand(int knownRev, const QString& diff, QObject * parent): CCommandBase(knownRev, parent)
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
