/*
 * IIdProvider.h
 *
 *  Created on: 23 Jun 2012
 *      Author: martin
 */

#ifndef IIDPROVIDER_H_
#define IIDPROVIDER_H_

#include <stdint.h>

namespace muroa {

class IIdProvider {
public:
	IIdProvider() {};
	virtual ~IIdProvider() {};

	virtual uint32_t getNextFreeID() = 0;
	virtual void setNextFreeID(uint32_t id) = 0;
};

} /* namespace muroa */
#endif /* IIDPROVIDER_H_ */
