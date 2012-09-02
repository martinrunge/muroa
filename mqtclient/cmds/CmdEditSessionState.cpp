/*
 * CmdEditSessionState.cpp
 *
 *  Created on: 02 Sep 2012
 *      Author: martin
 */

#include "CmdEditSessionState.h"

using namespace std;

CmdEditSessionState::CmdEditSessionState(unsigned knownRev, QObject * parent) : CmdBase(EDIT_SESSION_STATE, knownRev, parent)
{
	m_name = "editSessionState";
}

CmdEditSessionState::CmdEditSessionState(unsigned knownRev, const string& diff, QObject * parent): CmdBase(EDIT_SESSION_STATE, knownRev, parent)
{
	m_name = "editSessionState";
	m_data = diff;
}

CmdEditSessionState::~CmdEditSessionState()
{

}

void CmdEditSessionState::setData(const string& diff)
{
	m_data = diff;
}

void CmdEditSessionState::timeout()
{
	CmdBase::timeout();
}
