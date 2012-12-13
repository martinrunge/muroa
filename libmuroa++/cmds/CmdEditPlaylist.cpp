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
}

CmdEditPlaylist::CmdEditPlaylist(unsigned  fromRev, unsigned toRev, std::string diff)
                              : Cmd(Cmd::EDIT_PLAYLIST),
                                m_fromRev(fromRev),
                                m_toRev(toRev),
                                m_diff(diff)
{

}

CmdEditPlaylist::~CmdEditPlaylist() {
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

    unsigned CmdEditPlaylist::getToRev() const
    {
        return m_toRev;
    }

    void CmdEditPlaylist::setToRev(unsigned  toRev)
    {
        m_toRev = toRev;
    }


} /* namespace muroa */
