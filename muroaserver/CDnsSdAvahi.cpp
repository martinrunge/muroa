/*
 * CDnsSdAvahi.cpp
 *
 *  Created on: 23 Mar 2010
 *      Author: martin
 */

#include "CDnsSdAvahi.h"

#include <iostream>
#include <avahi-common/watch.h>
#include <avahi-common/error.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-qt4/qt-watch.h>

#include <assert.h>

using namespace std;


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



CDnsSdAvahi::CDnsSdAvahi() : m_client(0), m_group(0) {
}

CDnsSdAvahi::~CDnsSdAvahi() {
    if (m_client)
        avahi_client_free(m_client);

}

void CDnsSdAvahi::cleanup()
{

}

void CDnsSdAvahi::registerService(string serviceName, unsigned short servicePort)
{
	if(m_client)
	{
		cleanup();
	}

	m_serviceName = serviceName;
	m_servicePort = servicePort;
	m_userdata.thisPtr = this;
    /* Allocate a new client */
	int error;
    m_client = avahi_client_new(avahi_qt_poll_get(), AvahiClientFlags(0), &CDnsSdAvahi::staticClientCallback, &m_userdata, &error);

    /* Check wether creating the client object succeeded */
    if (!m_client) {
        cerr << "Failed to create client: " << avahi_strerror(error) << endl;
        cleanup();
        return;
    }
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
			;
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
			;
	}
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
