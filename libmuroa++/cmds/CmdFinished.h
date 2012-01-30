/*
 * CmdFinished.h
 *
 *  Created on: 30 Jan 2012
 *      Author: martin
 */

#ifndef CMDFINISHED_H_
#define CMDFINISHED_H_

#include "Cmd.h"

namespace muroa {

class CmdFinished: public muroa::Cmd {
public:
	CmdFinished();
	CmdFinished(uint32_t correspondingJobID);
	virtual ~CmdFinished();

    uint32_t getCorrespondingJobId() const;
    void setCorrespondingJobId(uint32_t correspondingJobId);
private:
    uint32_t m_correspondingJobID;
};

} /* namespace muroa */
#endif /* CMDFINISHED_H_ */
