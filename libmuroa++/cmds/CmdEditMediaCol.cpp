/*
 * CmdColChanged.cpp
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#include "CmdEditMediaCol.h"

using namespace std;

namespace muroa {

CmdEditMediaCol::CmdEditMediaCol() : Cmd(Cmd::EDIT_MEDIACOL) {
	// TODO Auto-generated constructor stub

}

CmdEditMediaCol::CmdEditMediaCol(unsigned  fromRev, std::string diff)
                              : Cmd(Cmd::EDIT_MEDIACOL),
                                m_fromRev(fromRev),
                                m_diff(diff)
{

}

CmdEditMediaCol::~CmdEditMediaCol() {
	// TODO Auto-generated destructor stub
}

    string CmdEditMediaCol::getDiff() const
    {
        return m_diff;
    }

    void CmdEditMediaCol::setDiff(string diff)
    {
        m_diff = diff;
    }

    unsigned CmdEditMediaCol::getFromRev() const
    {
        return m_fromRev;
    }

    void CmdEditMediaCol::setFromRev(unsigned  fromRev)
    {
        m_fromRev = fromRev;
    }

} /* namespace muroa */
