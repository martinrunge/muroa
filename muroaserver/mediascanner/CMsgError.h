/*
 * CMsgError.h
 *
 *  Created on: 11 Apr 2011
 *      Author: martin
 */

#ifndef CMSGERROR_H_
#define CMSGERROR_H_

#include "CMsgBase.h"

#include <string>

class CMsgError: public CMsgBase {
public:
	CMsgError(int32_t errorcode, std::string message);
	CMsgError(char* buffer, int size);

	virtual ~CMsgError();

	int32_t  getErrorCode() {return m_error_code; };
	std::string getMessage() { return m_message; };

	bool operator==(const CMsgError& other);

	char* serialize(int& size );

private:
	int32_t m_error_code;
	std::string m_message;


};

#endif /* CMSGERROR_H_ */
