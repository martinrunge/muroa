/*
 * CStreamClientDicovery.h
 *
 *  Created on: 10.01.2016
 *      Author: martin
 */

#ifndef LIBMSTREAM_CSTREAMCLIENTDISCOVERY_H_
#define LIBMSTREAM_CSTREAMCLIENTDISCOVERY_H_

#include <avahi/CDnsSdBase.h>
#include <boost/asio.hpp>

namespace muroa {

class CDnsSdAvahi;

class CStreamClientDiscovery {
public:
	CStreamClientDiscovery(boost::asio::io_service& io_service);
	virtual ~CStreamClientDiscovery();


protected:
	virtual void onClientAppeared(ServDescPtr srvPtr);
	virtual void onClientDisappeared(ServDescPtr srvPtr);
	virtual void onClientChanged();


private:
    CDnsSdAvahi* m_dnssd;

};

} /* namespace muroa */

#endif /* LIBMSTREAM_CSTREAMCLIENTDISCOVERY_H_ */
