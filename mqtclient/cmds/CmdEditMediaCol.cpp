/*
 * CCollectionCommand.cpp
 *
 *  Created on: 10 Mar 2010
 *      Author: martin
 */

#include "CmdEditMediaCol.h"

using namespace std;

CmdEditMediaCol::CmdEditMediaCol( unsigned knownRev, QObject * parent ) : CmdBase(EDIT_MEDIA_COL, knownRev, parent)
{
	m_name = "editMediaCol";
}


CmdEditMediaCol::CmdEditMediaCol( unsigned knownRev, const string& diff, QObject * parent ) : CmdBase(EDIT_MEDIA_COL, knownRev, parent)
{
	m_name = "editMediaCol";
	m_data = diff;
}

CmdEditMediaCol::~CmdEditMediaCol() {

}

void CmdEditMediaCol::setData(const string& diff)
{
	m_data = diff;
}

void CmdEditMediaCol::timeout()
{
	CmdBase::timeout();
}
