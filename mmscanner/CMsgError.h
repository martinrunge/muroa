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
	CMsgError(uint32_t requestID, int32_t errorcode, std::string message);
	CMsgError(char* buffer, int size);

	virtual ~CMsgError();

	uint32_t getRequestID() {return m_requestID; };
	int32_t  getErrorCode() {return m_error_code; };
	std::string getMessage() { return m_message; };

	bool operator==(const CMsgError& other);

	char* serialize(int& size );

private:
	uint32_t    m_requestID;
	int32_t     m_error_code;
	std::string m_message;


};

#endif /* CMSGERROR_H_ */
