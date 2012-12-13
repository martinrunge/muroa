/*
 * CmdEditNextlist.cpp
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#include "CmdEditNextlist.h"

using namespace std;

namespace muroa {

CmdEditNextlist::CmdEditNextlist() : Cmd(Cmd::EDIT_NEXTLIST) {
}

CmdEditNextlist::CmdEditNextlist(unsigned  fromRev, unsigned toRev, std::string diff)
                              : Cmd(Cmd::EDIT_NEXTLIST),
                                m_fromRev(fromRev),
                                m_toRev(toRev),
                                m_diff(diff)
{

}

CmdEditNextlist::~CmdEditNextlist() {
}

    string CmdEditNextlist::getDiff() const
    {
        return m_diff;
    }

    void CmdEditNextlist::setDiff(string diff)
    {
        m_diff = diff;
    }

    unsigned CmdEditNextlist::getFromRev() const
    {
        return m_fromRev;
    }

    void CmdEditNextlist::setFromRev(unsigned  fromRev)
    {
        m_fromRev = fromRev;
    }

    unsigned CmdEditNextlist::getToRev() const
    {
        return m_toRev;
    }

    void CmdEditNextlist::setToRev(unsigned  toRev)
    {
        m_toRev = toRev;
    }
} /* namespace muroa */
