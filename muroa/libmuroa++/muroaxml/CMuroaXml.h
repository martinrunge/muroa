/*
 * CMuroaXml.h
 *
 *  Created on: 12 Aug 2011
 *      Author: martin
 */

#ifndef CMUROAXML_H_
#define CMUROAXML_H_

#include "IRPC.h"
#include "CParserStateMachine.h"
#include "MuroaExceptions.h"

#include <expat.h>
#include <cstdint>

class CMuroaXml: public IRPC , public CParserStateMachine {
public:
	CMuroaXml() throw(rpcError);
	~CMuroaXml();

	void joinSession(uint32_t sessionID);
	void leaveSession();

	void play();
	void pause();
	void stop();
	void next();
	void prev();

	void stateChanged(int newState);
	void progress(uint32_t jobID, int progress);
	void error(uint32_t jobID, int errorCode, std::string description);

	void getCollection( unsigned knownRev = 0);
	void getPlaylist( unsigned knownRev = 0);
	void getNextlist( unsigned knownRev = 0);

	void editCollection( unsigned fromRev );
	void editPlaylist( unsigned fromRev );
	void editNextlist( unsigned fromRev );


	void newData(const char* data, int len);

private:
	void sendData(std::string data);

    static void XMLCALL startTagHandler(void *inst_ptr, const char *el, const char **attr);
    static void XMLCALL endTagHandler(void *inst_ptr, const char *el);
    static void XMLCALL characterHandler(void *inst_ptr, const char *s, int len);


	XML_Parser m_parser;

	uint32_t m_sessionID;
};

#endif /* CMUROAXML_H_ */
