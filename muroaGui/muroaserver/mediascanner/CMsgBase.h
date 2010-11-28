/*
 * CMsgBase.h
 *
 *  Created on: 31 Oct 2010
 *      Author: martin
 */

#ifndef CMSGBASE_H_
#define CMSGBASE_H_

#include <stdint.h>

enum msg_types {
	E_MSG_RESP,
	E_MSG_QUIT,
	E_MSG_OPEN_DB,
	E_MSG_SCAN_DIR,
	E_MSG_COLLECTION_CHANGED,
	E_MSG_FINISHED,
	E_MSG_PROGRESS
};

class CMsgBase {
public:
	CMsgBase();
	CMsgBase(char* buffer, int size);

	virtual ~CMsgBase();

	static CMsgBase* msgFactory(char* buffer, int size);

	uint32_t getType() {return m_msgType; };
	uint32_t getID() {return m_msgID; };
	uint32_t getPayloadSize() {return m_payloadSize; };

	virtual char* serialize(int& size ) = 0;
	bool equalTo(const CMsgBase &other);
	char* getSerialisationBufferPtr() { return m_serialisationBuffer; };
	char* getPayloadBufferPtr() { return m_serialisationBuffer + getHeaderSize(); };

protected:
	uint32_t m_msgType;
	uint32_t m_msgID;
	uint32_t m_payloadSize;

	int getHeaderSize() { return (3 * sizeof(uint32_t)); };
	void serializeHeader();
	int reallocSerialisationBuffer( int size = 0 );


	static int m_last_id;

	uint32_t  checkHeader(char* buffer, int size, enum msg_types type);

private:
	char* m_serialisationBuffer;
	int m_serialisationBufferSize;
};

#endif /* CMSGBASE_H_ */
