/*
 * CmdError.h
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#ifndef CMDERROR_H_
#define CMDERROR_H_

#include "Cmd.h"
#include <string>

namespace muroa {

class CmdError: public muroa::Cmd {
public:
	CmdError();
	CmdError(unsigned correspondingJobID, int errorCode, std::string description);
    virtual ~CmdError();

	int getErrorCode() const;
    void setErrorCode(int code);

    unsigned getCorrespondingJobID() const;
    void setCorrespondingJobID(unsigned  correspondingJobId);

    std::string getDescription() const;
    void setDescription(std::string description);

private:
	unsigned m_correspondingJobID;
	int m_errorCode;
	std::string m_description;
};

} /* namespace muroa */
#endif /* CMDERROR_H_ */
