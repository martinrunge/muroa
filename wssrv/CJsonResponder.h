/*
 * CJsonResponder.h
 *
 *  Created on: 13.12.2015
 *      Author: martin
 */

#ifndef CJSONRESPONDER_H_
#define CJSONRESPONDER_H_

#include <string>

namespace muroa {

class CJsonResponder {
public:
	CJsonResponder();
	virtual ~CJsonResponder();

	static std::string testOutput();
};

} /* namespace muroa */

#endif /* CJSONRESPONDER_H_ */
