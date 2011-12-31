/*
 * CmdProgress.cpp
 *
 *  Created on: 27 Dec 2011
 *      Author: martin
 */

#include "CmdProgress.h"

namespace muroa {

CmdProgress::CmdProgress() : Cmd(Cmd::PROGRESS) {
	// TODO Auto-generated constructor stub

}

CmdProgress::CmdProgress(uint32_t correspondingJobID, uint32_t progress)
    :Cmd(Cmd::PROGRESS), m_correspondingJobID(correspondingJobID), m_progress(progress)
    {
    }

    CmdProgress::~CmdProgress()
    {
        // TODO Auto-generated destructor stub
    }

    uint32_t CmdProgress::getCorrespondingJobId() const
    {
        return m_correspondingJobID;
    }

    uint32_t CmdProgress::getProgress() const
    {
        return m_progress;
    }

    void CmdProgress::setCorrespondingJobId(uint32_t correspondingJobId)
    {
        m_correspondingJobID = correspondingJobId;
    }

    void CmdProgress::setProgress(uint32_t progress)
    {
        m_progress = progress;
    }

} /* namespace muroa */

