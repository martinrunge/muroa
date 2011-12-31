/*
 * CmdColChanged.h
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#ifndef CMDCOLCHANGED_H_
#define CMDCOLCHANGED_H_

#include "Cmd.h"
#include <string>

namespace muroa {

class CmdColChanged: public muroa::Cmd {
public:
	CmdColChanged();
    CmdColChanged(unsigned  fromRev, std::string diff);
    virtual ~CmdColChanged();

    std::string getDiff() const;
    void setDiff(std::string diff);

    unsigned getFromRev() const;
    void setFromRev(unsigned  fromRev);

private:
    unsigned m_fromRev;
    std::string m_diff;
};

} /* namespace muroa */
#endif /* CMDCOLCHANGED_H_ */
