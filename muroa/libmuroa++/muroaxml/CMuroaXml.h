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

class CMuroaXml: public IRPC , public CParserStateMachine {
public:
	CMuroaXml() throw(rpcError);
	~CMuroaXml();

    void join(std::string host, unsigned port);
	void leave();

	virtual void onConnectionStateChanged(int state);

	void play();
	void stop();
	void next();
	void prev();

	void getCollection( int knownRev );
	void getPlaylist( int knownRev );
	void getNextlist( int knownRev );

	void editCollection( int fromRev );
	void editPlaylist( int fromRev );
	void editNextlist( int fromRev );

	virtual void onPlay();
	virtual void onStop();
	virtual void onNext();
	virtual void onPrev();

	virtual void onGetCollection( int knownRev );
	virtual void onGetPlaylist( int knownRev );
	virtual void onGetNextlist( int knownRev );

	virtual void onCollection( unsigned knownRev );
	virtual void onPlaylist( unsigned knownRev );
	virtual void onNextlist( unsigned knownRev );

	virtual void onEditCollection( int fromRev );
	virtual void onEditPlaylist( int fromRev );
	virtual void onEditNextlist( int fromRev );

private:
    void parseChunk(const char* buffer, const int len);

    static void XMLCALL startTagHandler(void *data, const char *el, const char **attr);
    static void XMLCALL endTagHandler(void *data, const char *el);


	XML_Parser m_parser;


};

#endif /* CMUROAXML_H_ */
