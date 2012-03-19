/*
 * CmdEditPlaylist.cpp
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#include "CmdEditPlaylist.h"

using namespace std;

namespace muroa {

CmdEditPlaylist::CmdEditPlaylist() : Cmd(Cmd::EDIT_PLAYLIST) {
	// TODO Auto-generated constructor stub

}

CmdEditPlaylist::CmdEditPlaylist(unsigned  fromRev, std::string diff)
                              : Cmd(Cmd::EDIT_PLAYLIST),
                                m_fromRev(fromRev),
                                m_diff(diff)
{

}

CmdEditPlaylist::~CmdEditPlaylist() {
	// TODO Auto-generated destructor stub
}

    string CmdEditPlaylist::getDiff() const
    {
        return m_diff;
    }

    void CmdEditPlaylist::setDiff(string diff)
    {
        m_diff = diff;
    }

    unsigned CmdEditPlaylist::getFromRev() const
    {
        return m_fromRev;
    }

    void CmdEditPlaylist::setFromRev(unsigned  fromRev)
    {
        m_fromRev = fromRev;
    }

} /* namespace muroa */
