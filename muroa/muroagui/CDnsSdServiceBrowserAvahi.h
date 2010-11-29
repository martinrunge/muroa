/*
 * CDnsSdServiceBrowserAvahi.h
 *
 *  Created on: 23 Mar 2010
 *      Author: martin
 */

#ifndef CDNSSDSERVICEBROWSERAVAHI_H_
#define CDNSSDSERVICEBROWSERAVAHI_H_

#include "CDnsSdBase.h"

#include <string>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

class CDnsSdServiceBrowserAvahi;

struct userdata
{
	CDnsSdServiceBrowserAvahi* thisPtr;
};


class CDnsSdServiceBrowserAvahi: public CDnsSdBase {
public:
	CDnsSdServiceBrowserAvahi(QString serviceType);
	virtual ~CDnsSdServiceBrowserAvahi();

	void clientCallback( AvahiClient *client, AvahiClientState state, void * userdata);
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

	static void staticClientCallback( AvahiClient *client, AvahiClientState state, void * userdata);
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

	struct userdata m_userdata;

    AvahiClient *m_client;
	AvahiServiceBrowser *m_serviceBrowser;

	QString m_serviceType;
};

#endif /* CDNSSDSERVICEBROWSERAVAHI_H_ */
