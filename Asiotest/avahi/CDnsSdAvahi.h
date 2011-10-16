/*
 * CDnsSdAvahi.h
 *
 *  Created on: 23 Mar 2010
 *      Author: martin
 */

#ifndef CDNSSDAVAHI_H_
#define CDNSSDAVAHI_H_

#include <boost/asio.hpp>

#include <string>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/simple-watch.h>

namespace muroa {

class CDnsSdAvahi;

struct userdata
{
	CDnsSdAvahi* thisPtr;
};


class CDnsSdAvahi {
public:
	CDnsSdAvahi(boost::asio::io_service& io_service);
	virtual ~CDnsSdAvahi();

	void operator()();
	void cancel();

	void registerService(std::string serviceName, unsigned short servicePort);

	void clientCallback(AvahiClient *client, AvahiClientState state, void * userdata);
	void entryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);

	static void staticClientCallback(AvahiClient *client, AvahiClientState state, void * userdata);
	static void staticEntryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);

private:
	void cleanup();
	void createService(AvahiClient *client);

	struct userdata m_userdata;

	AvahiSimplePoll *m_simple_poll;
    AvahiClient *m_client;
	AvahiEntryGroup *m_group;

	boost::asio::io_service& m_io_service;

	std::string m_serviceName;
	unsigned short m_servicePort;
};

} // namespace muroa
#endif /* CDNSSDAVAHI_H_ */
