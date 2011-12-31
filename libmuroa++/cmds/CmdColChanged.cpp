/*
 * CmdColChanged.cpp
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#include "CmdColChanged.h"

using namespace std;

namespace muroa {

CmdColChanged::CmdColChanged() : Cmd(Cmd::COLCHANGED) {
	// TODO Auto-generated constructor stub

}

CmdColChanged::CmdColChanged(unsigned  fromRev, std::string diff)
                              : Cmd(Cmd::COLCHANGED),
                                m_fromRev(fromRev),
                                m_diff(diff)
{

}

CmdColChanged::~CmdColChanged() {
	// TODO Auto-generated destructor stub
}

    string CmdColChanged::getDiff() const
    {
        return m_diff;
    }

    void CmdColChanged::setDiff(string diff)
    {
        m_diff = diff;
    }

    unsigned CmdColChanged::getFromRev() const
    {
        return m_fromRev;
    }

    void CmdColChanged::setFromRev(unsigned  fromRev)
    {
        m_fromRev = fromRev;
    }

} /* namespace muroa */
