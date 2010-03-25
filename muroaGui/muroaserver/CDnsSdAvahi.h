/*
 * CDnsSdAvahi.h
 *
 *  Created on: 23 Mar 2010
 *      Author: martin
 */

#ifndef CDNSSDAVAHI_H_
#define CDNSSDAVAHI_H_

#include "IDnsSd.h"

#include <string>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

class CDnsSdAvahi;

struct userdata
{
	CDnsSdAvahi* thisPtr;
};


class CDnsSdAvahi: public IDnsSd {
public:
	CDnsSdAvahi();
	virtual ~CDnsSdAvahi();

	void registerService(std::string serviceName, unsigned short servicePort);

	void clientCallback(AvahiClient *client, AvahiClientState state, void * userdata);
	void entryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);

	static void staticClientCallback(AvahiClient *client, AvahiClientState state, void * userdata);
	static void staticEntryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);

private:
	void cleanup();
	void createService(AvahiClient *client);

	struct userdata m_userdata;

    AvahiClient *m_client;
	AvahiEntryGroup *m_group;

	std::string m_serviceName;
	unsigned short m_servicePort;
};

#endif /* CDNSSDAVAHI_H_ */
