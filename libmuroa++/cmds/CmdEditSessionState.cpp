/*
 * CmdEditSessionState.cpp
 *
 *  Created on: 30.8.2012
 *      Author: martin
 */

#include "CmdEditSessionState.h"

using namespace std;

namespace muroa {

CmdEditSessionState::CmdEditSessionState() : Cmd(Cmd::EDIT_SESSION_STATE) {
	// TODO Auto-generated constructor stub

}

CmdEditSessionState::CmdEditSessionState(unsigned  fromRev, unsigned toRev, std::string diff)
                                         : Cmd(Cmd::EDIT_SESSION_STATE),
                                           m_fromRev(fromRev),
                                           m_toRev(toRev),
                                           m_diff(diff)
{

}

CmdEditSessionState::~CmdEditSessionState() {
	// TODO Auto-generated destructor stub
}

    string CmdEditSessionState::getDiff() const
    {
        return m_diff;
    }

    void CmdEditSessionState::setDiff(string diff)
    {
        m_diff = diff;
    }

    unsigned CmdEditSessionState::getFromRev() const
    {
        return m_fromRev;
    }

    void CmdEditSessionState::setFromRev(unsigned  fromRev)
    {
        m_fromRev = fromRev;
    }

    unsigned CmdEditSessionState::getToRev() const
    {
        return m_toRev;
    }

    void CmdEditSessionState::setToRev(unsigned  toRev)
    {
        m_toRev = toRev;
    }
} /* namespace muroa */
