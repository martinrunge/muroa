/*
 * CmdError.cpp
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#include "CmdError.h"

namespace muroa {

CmdError::CmdError()
    :Cmd(Cmd::ERROR)
    {
        // TODO Auto-generated constructor stub
    }

    CmdError::CmdError(unsigned  correspondingJobID, int errorCode, std::string description)
    :Cmd(Cmd::ERROR), m_correspondingJobID(correspondingJobID), m_errorCode(errorCode), m_description(description)
    {
    }

    CmdError::~CmdError()
    {
    }

    int CmdError::getErrorCode() const
    {
        return m_errorCode;
    }

    void CmdError::setErrorCode(int code)
    {
        m_errorCode = code;
    }

    unsigned CmdError::getCorrespondingJobID() const
    {
        return m_correspondingJobID;
    }

    void CmdError::setCorrespondingJobID(unsigned  correspondingJobId)
    {
        m_correspondingJobID = correspondingJobId;
    }

    std::string CmdError::getDescription() const
    {
        return m_description;
    }

    void CmdError::setDescription(std::string description)
    {
        m_description = description;
    }

} /* namespace muroa */

