/*
 * CNextlistCommand.cpp
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#include "CmdEditNextlist.h"

using namespace std;

CmdEditNextlist::CmdEditNextlist(unsigned knownRev, QObject * parent) : CmdBase(EDIT_NEXTLIST, knownRev, parent)
{
	m_name = "editNextlist";
}

CmdEditNextlist::CmdEditNextlist(unsigned knownRev, const string& diff, QObject * parent): CmdBase(EDIT_NEXTLIST, knownRev, parent)
{
	m_name = "editNextlist";
	m_data = diff;
}

CmdEditNextlist::~CmdEditNextlist()
{

}

void CmdEditNextlist::setData(const string& diff)
{
	m_data = diff;
}

void CmdEditNextlist::timeout()
{
	CmdBase::timeout();
}
