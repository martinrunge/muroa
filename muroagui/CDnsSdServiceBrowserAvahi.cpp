/*
 * CDnsSdServiceBrowserAvahi.cpp
 *
 *  Created on: 23 Mar 2010
 *      Author: martin
 */

#include "CDnsSdServiceBrowserAvahi.h"

#include <iostream>
#include <avahi-common/watch.h>
#include <avahi-common/error.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-qt4/qt-watch.h>

#include <assert.h>

using namespace std;


void CDnsSdServiceBrowserAvahi::staticClientCallback(AvahiClient *client, AvahiClientState state, void * userdata)
{
	struct userdata*  udata = (struct userdata*) userdata;
	CDnsSdServiceBrowserAvahi* instPtr = udata->thisPtr;
	return instPtr->clientCallback(client, state, userdata);
}


void CDnsSdServiceBrowserAvahi::staticBrowseCallback(AvahiServiceBrowser *b,
		                                             AvahiIfIndex interface,
		                                             AvahiProtocol protocol,
		                                             AvahiBrowserEvent event,
		                                             const char *name,
		                                             const char *type,
		                                             const char *domain,
		                                             AvahiLookupResultFlags flags,
		                                             void* userdata)
{
	struct userdata*  udata = (struct userdata*) userdata;
	CDnsSdServiceBrowserAvahi* instPtr = udata->thisPtr;
	return instPtr->browseCallback(b, interface, protocol, event, name, type, domain, flags, userdata);
}


void CDnsSdServiceBrowserAvahi::staticResolveCallback( AvahiServiceResolver *r,
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
                                                              void* userdata)
{
	struct userdata*  udata = (struct userdata*) userdata;
	CDnsSdServiceBrowserAvahi* instPtr = udata->thisPtr;
	return instPtr->resolveCallback(r, interface, protocol, event, name, type, domain, host_name, address, port, txt, flags, userdata);
}



CDnsSdServiceBrowserAvahi::CDnsSdServiceBrowserAvahi(QString serviceType) : m_serviceType(serviceType), m_client(0), m_serviceBrowser(0)
{
	m_userdata.thisPtr = this;
    /* Allocate a new client */
	int error;
    m_client = avahi_client_new(avahi_qt_poll_get(), AvahiClientFlags(0), &CDnsSdServiceBrowserAvahi::staticClientCallback, &m_userdata, &error);

    /* Check if creating the client object succeeded */
    if (!m_client) {
        cerr << "Failed to create client: " << avahi_strerror(error) << endl;
        return;
    }

    // Create the service browser
    m_serviceBrowser = avahi_service_browser_new(m_client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, m_serviceType.toUtf8(), NULL, AvahiLookupFlags(0), &CDnsSdServiceBrowserAvahi::staticBrowseCallback, &m_userdata);
    if(!m_serviceBrowser)
    {
      cerr << "Failed to create service browser: "<< avahi_strerror(avahi_client_errno(m_client)) << endl;
      return;
    }
}

CDnsSdServiceBrowserAvahi::~CDnsSdServiceBrowserAvahi() {
    if(m_serviceBrowser)
    {
    	avahi_service_browser_free(m_serviceBrowser);
    }

    if (m_client)
    {
        avahi_client_free(m_client);
    }
}



void CDnsSdServiceBrowserAvahi::clientCallback(AvahiClient *client, AvahiClientState state, void * userdata)
{

	/* Called whenever the client or server state changes */

	switch (state) {
		case AVAHI_CLIENT_S_RUNNING:

			/* The server has startup successfully and registered its host
			 * name on the network, so it's time to create our services */
			break;

		case AVAHI_CLIENT_FAILURE:

			cerr << "Client failure: " << avahi_strerror(avahi_client_errno(m_client)) << endl;
			break;

		case AVAHI_CLIENT_S_COLLISION:

			/* Let's drop our registered services. When the server is back
			 * in AVAHI_SERVER_RUNNING state we will register them
			 * again with the new host name. */

		case AVAHI_CLIENT_S_REGISTERING:

			/* The server records are now being established. This
			 * might be caused by a host name change. We need to wait
			 * for our own records to register until the host name is
			 * properly established. */

			break;

		case AVAHI_CLIENT_CONNECTING:
			;
	}
}


void CDnsSdServiceBrowserAvahi::browseCallback(AvahiServiceBrowser *b,
		            AvahiIfIndex interface,
		            AvahiProtocol protocol,
		            AvahiBrowserEvent event,
		            const char *name,
		            const char *type,
		            const char *domain,
		            AvahiLookupResultFlags flags,
		            void* userdata)
{
    assert(b);

    /* Called whenever a new services becomes available on the LAN or is removed from the LAN */

    switch (event) {
        case AVAHI_BROWSER_FAILURE:

            cerr << "(DnsSD Browser): "<< avahi_strerror(avahi_client_errno(avahi_service_browser_get_client(b))) << endl;
            return;

        case AVAHI_BROWSER_NEW:
            cerr << "(Browser) NEW: service '" << name <<"' of type '" << type << "' in domain '" << domain << "'" << endl;

            /* We ignore the returned resolver object. In the callback
               function we free it. If the server is terminated before
               the callback function is called the server will free
               the resolver for us. */

            if (!(avahi_service_resolver_new(m_client, interface, protocol, name, type, domain, AVAHI_PROTO_UNSPEC, AvahiLookupFlags(0), &CDnsSdServiceBrowserAvahi::staticResolveCallback, userdata)))
                cerr << "Failed to resolve service '" << name << "': " << avahi_strerror(avahi_client_errno(m_client)) << endl;

            break;

        case AVAHI_BROWSER_REMOVE:
        {
            cerr << "(Browser) REMOVE: service '" << name << "' of type '" << type << "' in domain '" << domain << "'" << endl;
            CServiceDesc tmpSd(name, "", domain, 0, interface, protocol);
            removeService( tmpSd );
            emit servicesChanged();
            break;
        }

        case AVAHI_BROWSER_ALL_FOR_NOW:
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            cerr << "(Browser) " << (event == AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" : "ALL_FOR_NOW") << endl;
            break;
    }
}


void CDnsSdServiceBrowserAvahi::resolveCallback( AvahiServiceResolver *r,
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
                                                 void* userdata)
{
    assert(r);
    /* Called whenever a service has been resolved successfully or timed out */

    switch (event) {
        case AVAHI_RESOLVER_FAILURE:
            cerr << "(Resolver) Failed to resolve service '" << name << "' of type '" << type << "' in domain '" << domain << "':" << avahi_strerror(avahi_client_errno(avahi_service_resolver_get_client(r))) << endl;
            break;

        case AVAHI_RESOLVER_FOUND: {
//            char a[AVAHI_ADDRESS_STR_MAX], *t;
//            avahi_address_snprint(a, sizeof(a), address);

            cerr << "Service '" << name << "' of type '" << type << "' in domain '" << domain << "':" << endl;
            cerr << "Host: " << host_name << " Port: " << port << endl;

            if(hasService(name) == 0)
            {
            	// no service with that name known yet
            	addService(new CServiceDesc(name, host_name, domain, port, interface, protocol));
            	emit servicesChanged();
            }


//            t = avahi_string_list_to_string(txt);
//            fprintf(stderr,
//                    "\t%s:%u (%s)\n"
//                    "\tTXT=%s\n"
//                    "\tcookie is %u\n"
//                    "\tis_local: %i\n"
//                    "\tour_own: %i\n"
//                    "\twide_area: %i\n"
//                    "\tmulticast: %i\n"
//                    "\tcached: %i\n",
//                    host_name, port, a,
//                    t,
//                    avahi_string_list_get_service_cookie(txt),
//                    !!(flags & AVAHI_LOOKUP_RESULT_LOCAL),
//                    !!(flags & AVAHI_LOOKUP_RESULT_OUR_OWN),
//                    !!(flags & AVAHI_LOOKUP_RESULT_WIDE_AREA),
//                    !!(flags & AVAHI_LOOKUP_RESULT_MULTICAST),
//                    !!(flags & AVAHI_LOOKUP_RESULT_CACHED));

//            avahi_free(t);
        }
    }
    avahi_service_resolver_free(r);
}
