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
	CMsgFinished(uint32_t jobID);
	CMsgFinished(char* buffer, int size);
	virtual ~CMsgFinished();

	uint32_t getJobID() { return m_jobID; };

	char* serialize(int& size );
	bool operator==(const CMsgFinished& other);


private:
	uint32_t m_jobID;

};

#endif /* CMSGFINISHED_H_ */
