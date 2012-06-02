/*
 * CmdProgress.h
 *
 *  Created on: 27 Dec 2011
 *      Author: martin
 */

#ifndef CMDPROGRESS_H_
#define CMDPROGRESS_H_

#include "Cmd.h"

namespace muroa {

class CmdProgress: public muroa::Cmd {
public:
	CmdProgress();
	CmdProgress(uint32_t correspondingJobID, uint32_t done, uint32_t total = 100);
	virtual ~CmdProgress();

    uint32_t getCorrespondingJobId() const;
    void setCorrespondingJobId(uint32_t correspondingJobId);

    uint32_t getProgress() const;
    uint32_t getTotal() const;
    void setProgress(uint32_t done, uint32_t total = 100);
private:
    uint32_t m_correspondingJobID;
    uint32_t m_done;
    uint32_t m_total;
};

} /* namespace muroa */
#endif /* CMDPROGRESS_H_ */
