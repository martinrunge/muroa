/*
 * CmdEditSessionState.h
 *
 *  Created on: 30.8.2012
 *      Author: martin
 */

#ifndef CMD_EDIT_SESSION_STATE_H_
#define CMD_EDIT_SESSION_STATE_H_

#include "Cmd.h"
#include <string>

namespace muroa {

class CmdEditSessionState: public muroa::Cmd {
public:
	CmdEditSessionState();
	CmdEditSessionState(unsigned  fromRev, unsigned toRev, std::string diff);
    virtual ~CmdEditSessionState();

    std::string getDiff() const;
    void setDiff(std::string diff);

    unsigned getFromRev() const;
    void setFromRev(unsigned  fromRev);

    unsigned getToRev() const;
    void setToRev(unsigned  fromRev);
private:
    unsigned m_fromRev;
    unsigned m_toRev;
    std::string m_diff;

};

} /* namespace muroa */
#endif /* CMD_EDIT_SESSION_STATE_H_ */
