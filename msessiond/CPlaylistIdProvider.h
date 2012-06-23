/*
 * CPlaylistIdProvider.h
 *
 *  Created on: 23 Jun 2012
 *      Author: martin
 */

#ifndef CPLAYLISTIDPROVIDER_H_
#define CPLAYLISTIDPROVIDER_H_

#include <IIdProvider.h>

class CSession;

class CPlaylistIdProvider: public muroa::IIdProvider {
public:
	CPlaylistIdProvider(CSession* session);
	virtual ~CPlaylistIdProvider();

    uint32_t getNextFreeID();
	void setNextFreeID(uint32_t id);

private:
	CSession* m_session;
};

#endif /* CPLAYLISTIDPROVIDER_H_ */
