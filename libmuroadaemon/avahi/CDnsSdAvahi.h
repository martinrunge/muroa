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
#include <avahi-client/lookup.h>
#include <avahi-common/thread-watch.h>

#include "CDnsSdBase.h"
#include "CServiceDesc.h"

namespace muroa {

class CDnsSdAvahi;

struct userdata
{
	CDnsSdAvahi* thisPtr;
};


class CDnsSdAvahi : public CDnsSdBase {
public:
	CDnsSdAvahi(boost::asio::io_service& io_service, std::string service_name, unsigned short service_port, std::string service_type, std::vector<std::string> browselist);
	virtual ~CDnsSdAvahi();

//	void operator()();
//	void cancel();

//	void registerService(std::string serviceName, unsigned short servicePort);

	void addBrowseService(std::string sericeType);

	void clientCallback(AvahiClient *client, AvahiClientState state, void * userdata);
	void entryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);
	void browseCallback( AvahiServiceBrowser *b,
			             AvahiIfIndex interface,
			             AvahiProtocol protocol,
			             AvahiBrowserEvent event,
			             const char *name,
			             const char *type,
			             const char *domain,
			             AvahiLookupResultFlags flags,
			             void* userdata);

	void resolveCallback( AvahiServiceResolver *r,
	                      AvahiIfIndex interface,
	                      AvahiProtocol protocol,
	                      AvahiResolverEvent event,
	                      const char *name,
	                      const char *type,
	                      const char *domain,
	                      const char *host_name,
	                      const AvahiAddress *address,
	                      uint16_t port,
	                      AvahiStringList *txt,
	                      AvahiLookupResultFlags flags,
	                      void* userdata);

	static void staticClientCallback(AvahiClient *client, AvahiClientState state, void * userdata);
	static void staticEntryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);
	static void staticBrowseCallback( AvahiServiceBrowser *b,
                                      AvahiIfIndex interface,
                                      AvahiProtocol protocol,
                                      AvahiBrowserEvent event,
                                      const char *name,
                                      const char *type,
                                      const char *domain,
                                      AvahiLookupResultFlags flags,
                                      void* userdata);

	static void staticResolveCallback( AvahiServiceResolver *r,
	                                   AvahiIfIndex interface,
	                                   AvahiProtocol protocol,
	                                   AvahiResolverEvent event,
	                                   const char *name,
	                                   const char *type,
	                                   const char *domain,
	                                   const char *host_name,
	                                   const AvahiAddress *address,
	                                   uint16_t port,
	                                   AvahiStringList *txt,
	                                   AvahiLookupResultFlags flags,
	                                   void* userdata);


private:
	void cleanup();
	void createService(AvahiClient *client);

	struct userdata m_userdata;

	AvahiThreadedPoll *m_threaded_poll;
    AvahiClient *m_client;
	AvahiEntryGroup *m_group;
	AvahiServiceBrowser *m_service_browser;

	std::string m_serviceName;
	std::string m_service_type;
	unsigned short m_servicePort;
};

} // namespace muroa
#endif /* CDNSSDAVAHI_H_ */
