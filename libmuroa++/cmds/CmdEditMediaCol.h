/*
 * CmdColChanged.h
 *
 *  Created on: 28 Dec 2011
 *      Author: martin
 */

#ifndef CMD_EDIT_MEDIACOL_H_
#define CMD_EDIT_MEDIACOL_H_

#include "Cmd.h"
#include <string>

namespace muroa {

class CmdEditMediaCol: public muroa::Cmd {
public:
	CmdEditMediaCol();
    CmdEditMediaCol(unsigned  fromRev, std::string diff);
    virtual ~CmdEditMediaCol();

    std::string getDiff() const;
    void setDiff(std::string diff);

    unsigned getFromRev() const;
    void setFromRev(unsigned  fromRev);

private:
    unsigned m_fromRev;
    std::string m_diff;
};

} /* namespace muroa */
#endif /* CMD_EDIT_MEDIACOL_H_ */
