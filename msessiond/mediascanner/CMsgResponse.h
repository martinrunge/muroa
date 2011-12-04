/*
 * CMsgResponse.h
 *
 *  Created on: 31 Oct 2010
 *      Author: martin
 */

#ifndef CMSGRESPONSE_H_
#define CMSGRESPONSE_H_

#include "CMsgBase.h"

#include <string>

class CMsgResponse: public CMsgBase {
public:
	CMsgResponse(int requestID, int retCode, std::string message);
	CMsgResponse(char* buffer, int size);
	virtual ~CMsgResponse();

	bool operator==(const CMsgResponse &other);

	uint32_t getRequestID() {return m_requestID; };
	int32_t  getReturnCode() {return m_retCode; };
	std::string getMessage() { return m_message; };

	char* serialize(int& size );

private:
	uint32_t    m_requestID;
	int32_t     m_retCode;
	std::string m_message;
};

#endif /* CMSGRESPONSE_H_ */
