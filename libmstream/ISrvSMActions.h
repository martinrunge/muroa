/*
 * ISrvSMActions.h
 *
 *  Created on: 12.09.2015
 *      Author: martin
 */

#ifndef LIBMSTREAM_ISRVSMACTIONS_H_
#define LIBMSTREAM_ISRVSMACTIONS_H_

#include <string>

namespace muroa {

class CmdStreamBase;

class ISrvSMActions {
public:
	ISrvSMActions() {};
	virtual ~ISrvSMActions() {};

	virtual void reportError(std::string msg) = 0;
	virtual void reportTimeout(std::string msg) = 0;
	virtual void reportClientState(const CmdStreamBase* evt) = 0;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_ISRVSMACTIONS_H_ */
