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
	CMuroaXml() throw(ExRpcError);
	~CMuroaXml();

	void open();
	void close();

	void listSessions(std::vector<std::string> sessionList = std::vector<std::string>());

	void joinSession(std::string sessionName);

	void leaveSession();

	void play(uint32_t jobID);
	void pause(uint32_t jobID);
	void stop(uint32_t jobID);
	void next(uint32_t jobID);
	void prev(uint32_t jobID);

	void stateChanged(int newState);
	void scanCollection(uint32_t jobID);
	void progress(uint32_t jobID, int progress);
	void finished(uint32_t jobID);
	void error(uint32_t jobID, int errorCode, std::string description);

	void getCollection(uint32_t jobID, unsigned knownRev = 0);
	void getPlaylist(uint32_t jobID, unsigned knownRev = 0);
	void getNextlist(uint32_t jobID, unsigned knownRev = 0);

	void editCollection(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string diff );
	void editPlaylist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string diff );
	void editNextlist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string diff );


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
