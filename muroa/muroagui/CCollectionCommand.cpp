/*
 * CCollectionCommand.cpp
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#include "CCollectionCommand.h"

CCollectionCommand::CCollectionCommand( int knownRev, QObject * parent ) : CCommandBase(knownRev, parent)
{
	m_cmdName = "modCollection";
}


CCollectionCommand::CCollectionCommand( int knownRev, const QString& diff, QObject * parent ) : CCommandBase(knownRev, parent)
{
	m_cmdName = "modCollection";
	m_cmdData = diff;
}

CCollectionCommand::~CCollectionCommand() {

}

void CCollectionCommand::setData(const QString& diff)
{
	m_cmdData = diff;
}

void CCollectionCommand::timeout()
{
	CCommandBase::timeout();
}

