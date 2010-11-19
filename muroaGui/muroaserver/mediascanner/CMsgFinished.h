/*
 * CMsgFinished.h
 *
 *  Created on: 15 Nov 2010
 *      Author: martin
 */

#ifndef CMSGFINISHED_H_
#define CMSGFINISHED_H_

#include "CMsgBase.h"

class CMsgFinished: public CMsgBase {
public:
	CMsgFinished();
	CMsgFinished(char* buffer, int size);
	virtual ~CMsgFinished();

	char* serialize(int& size );
	bool operator==(const CMsgFinished& other);


private:
	// uint32_t m_jobID;   // tbd

};

#endif /* CMSGFINISHED_H_ */
