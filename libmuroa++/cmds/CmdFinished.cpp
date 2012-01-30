/*
 * CmdFinished.cpp
 *
 *  Created on: 30 Jan 2012
 *      Author: martin
 */

#include "CmdFinished.h"

namespace muroa {

	CmdFinished::CmdFinished() : Cmd(Cmd::FINISHED) {
	}

	CmdFinished::CmdFinished(uint32_t correspondingJobID)
							 : Cmd(Cmd::FINISHED), m_correspondingJobID(correspondingJobID)
   {

   }

    CmdFinished::~CmdFinished() {

    }

    uint32_t CmdFinished::getCorrespondingJobId() const {
    	return m_correspondingJobID;
    }

    void CmdFinished::setCorrespondingJobId(uint32_t correspondingJobId) {
    	m_correspondingJobID = correspondingJobId;
    }

} /* namespace muroa */
