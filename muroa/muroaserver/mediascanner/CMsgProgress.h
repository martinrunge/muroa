/*
 * CMsgProgress.h
 *
 *  Created on: 15 Nov 2010
 *      Author: martin
 */

#ifndef CMSGPROGRESS_H_
#define CMSGPROGRESS_H_

#include "CMsgBase.h"

class CMsgProgress: public CMsgBase {
public:
	CMsgProgress(uint32_t progress = 0);
	CMsgProgress(char* buffer, int size);
	virtual ~CMsgProgress();

	uint32_t getProgress() { return m_progress; };

	char* serialize(int& size );

	bool operator==(const CMsgProgress& other);

private:
	uint32_t m_progress;

};

#endif /* CMSGPROGRESS_H_ */
