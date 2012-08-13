/*
 * CDnsSdAvahi.cpp
 *
 *  Created on: 23 Mar 2010
 *      Author: martin
 */

#include "CDnsSdAvahi.h"
#include "../Exceptions.h"

#include <iostream>
#include <avahi-common/watch.h>
#include <avahi-common/error.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-common/thread-watch.h>

#include <assert.h>

using namespace std;

namespace muroa {

void CDnsSdAvahi::staticClientCallback(AvahiClient *client, AvahiClientState state, void * userdata)
{
	struct userdata*  udata = (struct userdata*) userdata;
	CDnsSdAvahi* instPtr = udata->thisPtr;
	return instPtr->clientCallback(client, state, userdata);
}

void CDnsSdAvahi::staticEntryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata)
{
	struct userdata*  udata = (struct userdata*) userdata;
	CDnsSdAvahi* instPtr = udata->thisPtr;
	return instPtr->entryGroupCallback(group, state, userdata);
}

void CDnsSdAvahi::staticBrowseCallback(AvahiServiceBrowser *b,
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
	CDnsSdAvahi* instPtr = udata->thisPtr;
	return instPtr->browseCallback(b, interface, protocol, event, name, type, domain, flags, userdata);
}


void CDnsSdAvahi::staticResolveCallback( AvahiServiceResolver *r,
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
	CDnsSdAvahi* instPtr = udata->thisPtr;
	return instPtr->resolveCallback(r, interface, protocol, event, name, type, domain, host_name, address, port, txt, flags, userdata);
}



CDnsSdAvahi::CDnsSdAvahi(boost::asio::io_service& io_service,
		                 string service_name,
		                 unsigned short service_port,
		                 string service_type)

                       : CDnsSdBase(io_service),
                         m_threaded_poll(0),
                         m_client(0),
                         m_group(0)
{

	m_userdata.thisPtr = this;
	m_serviceName = service_name;
	m_servicePort = service_port;
    m_service_type = service_type;


	/* Allocate main loop object */
    if (!(m_threaded_poll = avahi_threaded_poll_new())) {
        ostringstream oss;
        oss << "Failed to create simple poll object." << endl;

        throw(muroa::configEx(oss.str()));
    }
    int error;
    m_client = avahi_client_new(avahi_threaded_poll_get(m_threaded_poll), AvahiClientFlags(0), &CDnsSdAvahi::staticClientCallback, &m_userdata, &error);

    // Create the service browser
    m_service_browser = avahi_service_browser_new(m_client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, m_service_type.c_str(), NULL, AvahiLookupFlags(0), &CDnsSdAvahi::staticBrowseCallback, &m_userdata);
    if(!m_service_browser)
    {
      cerr << "Failed to create service browser: "<< avahi_strerror(avahi_client_errno(m_client)) << endl;
      return;
    }

	avahi_threaded_poll_start(m_threaded_poll);
}

CDnsSdAvahi::~CDnsSdAvahi() {
    if(m_threaded_poll) {
        avahi_threaded_poll_stop(m_threaded_poll);
        avahi_client_free(m_client);
        avahi_threaded_poll_free(m_threaded_poll);
        m_threaded_poll = 0;
	}
    /* Call this when the app shuts down */

}

void CDnsSdAvahi::cleanup()
{

}




void CDnsSdAvahi::clientCallback(AvahiClient *client, AvahiClientState state, void * userdata)
{

	/* Called whenever the client or server state changes */

	switch (state) {
		case AVAHI_CLIENT_S_RUNNING:

			/* The server has startup successfully and registered its host
			 * name on the network, so it's time to create our services */
			createService(client);
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
			 * properly esatblished. */

			if (m_group)
				avahi_entry_group_reset(m_group);

			break;

		case AVAHI_CLIENT_CONNECTING:
			break;
	}
}

void CDnsSdAvahi::entryGroupCallback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata)
{
	assert(group == m_group || m_group == NULL);
	m_group = group;

	/* Called whenever the entry group state changes */

	switch (state) {
		case AVAHI_ENTRY_GROUP_ESTABLISHED :
			/* The entry group has been established successfully */
			cerr << "Service ' " << m_serviceName << "' successfully established." << endl;
			break;

		case AVAHI_ENTRY_GROUP_COLLISION : {
			char *n;

			/* A service name collision with a remote service
			 * happened. Let's pick a new name */
			n = avahi_alternative_service_name(m_serviceName.c_str());
			m_serviceName = n;

			cerr << "Service name collision, renaming service to '" << m_serviceName << "'" << endl;

			/* And recreate the services */
			createService(m_client);
			break;
		}

		case AVAHI_ENTRY_GROUP_FAILURE :

			cerr << "Entry group failure: " << avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(group))) << endl;

			/* Some kind of failure happened while we were registering our services */
			break;

		case AVAHI_ENTRY_GROUP_UNCOMMITED:
		case AVAHI_ENTRY_GROUP_REGISTERING:
			break;
	}
}

void CDnsSdAvahi::browseCallback(AvahiServiceBrowser *b,
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

            if (!(avahi_service_resolver_new(m_client, interface, protocol, name, type, domain, AVAHI_PROTO_UNSPEC, AvahiLookupFlags(0), &CDnsSdAvahi::staticResolveCallback, userdata)))
                cerr << "Failed to resolve service '" << name << "': " << avahi_strerror(avahi_client_errno(m_client)) << endl;

            break;

        case AVAHI_BROWSER_REMOVE:
        {
            cerr << "(Browser) REMOVE: service '" << name << "' of type '" << type << "' in domain '" << domain << "'" << endl;
            CServiceDesc tmpSd(name, "", domain, 0, interface, protocol);
            removeService( tmpSd );
            break;
        }

        case AVAHI_BROWSER_ALL_FOR_NOW:
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            cerr << "(Browser) " << (event == AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" : "ALL_FOR_NOW") << endl;
            break;
    }
}


void CDnsSdAvahi::resolveCallback( AvahiServiceResolver *r,
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
            	addService(ServDescPtr( new CServiceDesc(name, host_name, domain, port, interface, protocol)));
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


void CDnsSdAvahi::createService(AvahiClient *client)
{
    char *n;
    int ret;

    /* If this is the first time we're called, let's create a new
     * entry group if necessary */

    if (!m_group)
    {
        if (!(m_group = avahi_entry_group_new(client, &CDnsSdAvahi::staticEntryGroupCallback, &m_userdata)))
        {
            cerr << "avahi_entry_group_new() failed: " << avahi_strerror(avahi_client_errno(client)) << endl;
        }
    }
    /* If the group is empty (either because it was just created, or
     * because it was reset previously, add our entries.  */

    if (avahi_entry_group_is_empty(m_group)) {
        cerr << "Adding service '" << m_serviceName << "'" << endl;

        /* Create some random TXT data */


        /* We will now add two services and one subtype to the entry
         * group. The two services have the same name, but differ in
         * the service type (IPP vs. BSD LPR). Only services with the
         * same name should be put in the same entry group. */

        /* Add the service for IPP */
        if ((ret = avahi_entry_group_add_service(m_group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, AvahiPublishFlags(0), m_serviceName.c_str(), "_muroa._tcp", NULL, NULL, m_servicePort, "role=session server", NULL)) < 0) {

            if (ret == AVAHI_ERR_COLLISION)
                goto collision;

            cerr << "Failed to add _ipp._tcp service: " << avahi_strerror(ret) << endl;
        }

        /* Tell the server to register the service */
        if ((ret = avahi_entry_group_commit(m_group)) < 0) {
            cerr <<  "Failed to commit entry group: " << avahi_strerror(ret) << endl;
        }
    }

    return;

collision:

    /* A service name collision with a local service happened. Let's
     * pick a new name */
    n = avahi_alternative_service_name(m_serviceName.c_str());
    m_serviceName = n;
    avahi_free(n);

    cerr << "Service name collision, renaming service to '" << m_serviceName << "'" << endl;

    avahi_entry_group_reset(m_group);

    createService(client);
    return;
}


//
//static void modify_callback(AVAHI_GCC_UNUSED AvahiTimeout *e, void *userdata) {
//    AvahiClient *client = userdata;
//
//    fprintf(stderr, "Doing some weird modification\n");
//
//    avahi_free(name);
//    name = avahi_strdup("Modified MegaPrinter");
//
//    /* If the server is currently running, we need to remove our
//     * service and create it anew */
//    if (avahi_client_get_state(client) == AVAHI_CLIENT_S_RUNNING) {
//
//        /* Remove the old services */
//        if (group)
//            avahi_entry_group_reset(group);
//
//        /* And create them again with the new name */
//        create_services(client);
//    }
//}
//

} // namespace muroa

