/*
 * CPlaylistIdProvider.h
 *
 *  Created on: 23 Jun 2012
 *      Author: martin
 */

#ifndef CPLAYLISTIDPROVIDER_H_
#define CPLAYLISTIDPROVIDER_H_

#include <IIdProvider.h>

namespace muroa {
class CSession;
}

class CPlaylistIdProvider: public muroa::IIdProvider {
public:
	CPlaylistIdProvider(muroa::CSession* session);
	virtual ~CPlaylistIdProvider();

    uint32_t getNextFreeID();
	void setNextFreeID(uint32_t id);

private:
	muroa::CSession* m_session;
	static uint32_t m_next_free_id;
};

#endif /* CPLAYLISTIDPROVIDER_H_ */
