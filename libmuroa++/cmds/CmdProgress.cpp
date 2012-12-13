/*
 * CmdProgress.cpp
 *
 *  Created on: 27 Dec 2011
 *      Author: martin
 */

#include "CmdProgress.h"

namespace muroa {

CmdProgress::CmdProgress() : Cmd(Cmd::PROGRESS) {
}

CmdProgress::CmdProgress(uint32_t correspondingJobID, uint32_t done, uint32_t total)
    :Cmd(Cmd::PROGRESS), m_correspondingJobID(correspondingJobID), m_done(done), m_total(total)
    {
    }

    CmdProgress::~CmdProgress()
    {
    }

    uint32_t CmdProgress::getCorrespondingJobId() const
    {
        return m_correspondingJobID;
    }

    uint32_t CmdProgress::getProgress() const
    {
        return m_done;
    }

    uint32_t CmdProgress::getTotal() const
    {
        return m_total;
    }

    void CmdProgress::setCorrespondingJobId(uint32_t correspondingJobId)
    {
        m_correspondingJobID = correspondingJobId;
    }

    void CmdProgress::setProgress(uint32_t done, uint32_t total)
    {
        m_done = done;
        m_total = total;
    }

} /* namespace muroa */

