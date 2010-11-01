/*
 * CMsgQuit.h
 *
 *  Created on: 31 Oct 2010
 *      Author: martin
 */

#ifndef CMSGQUIT_H_
#define CMSGQUIT_H_

#include "CMsgBase.h"

class CMsgQuit: public CMsgBase {
public:
	CMsgQuit();
	CMsgQuit(char* buffer, int size);
	virtual ~CMsgQuit();

	bool operator==(const CMsgQuit& other);

	char* serialize(int& size );
};

#endif /* CMSGQUIT_H_ */
