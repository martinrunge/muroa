/*
 * CMsgCollectionChanged.h
 *
 *  Created on: 6 Jan 2011
 *      Author: martin
 */

#ifndef CMSGCOLLECTIONCHANGED_H_
#define CMSGCOLLECTIONCHANGED_H_

#include "CMsgBase.h"

class CMsgCollectionChanged: public CMsgBase {
public:
	CMsgCollectionChanged(uint32_t newRev = 0, uint32_t minRev = 0, uint32_t maxRev = 0);
	CMsgCollectionChanged(char* buffer, int size);
	virtual ~CMsgCollectionChanged();

	uint32_t getNewRev() { return m_newRev; };
	uint32_t getMinRev() { return m_minRev; };
	uint32_t getMaxRev() { return m_maxRev; };

	char* serialize(int& size );

	bool operator==(const CMsgCollectionChanged& other);

private:
	uint32_t m_newRev;
	uint32_t m_minRev;
	uint32_t m_maxRev;
};

#endif /* CMSGCOLLECTIONCHANGED_H_ */
