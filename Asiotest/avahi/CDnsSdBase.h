/*
 * CDnsSdBase.h
 *
 *  Created on: 25 Mar 2010
 *      Author: martin
 */

#ifndef IDNSSD_H_
#define IDNSSD_H_

#include <vector>
#include <string>
#include <mutex>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

#include "CServiceDesc.h"
#include "CApp.h"

namespace muroa {

typedef boost::function<void(ServDescPtr sd_ptr)> notify_handler_t;
typedef boost::function<void(void)> service_changed_handler_t;

class CDnsSdBase {
public:
	CDnsSdBase(boost::asio::io_service& io_service);
	virtual ~CDnsSdBase();

    std::vector<ServDescPtr> getServiceList() const;
    void setServiceList(std::vector<ServDescPtr> services);

    ServDescPtr getService(std::string name, int which = 0);
    ServDescPtr getService(int index) { return m_services.at(index); };

    void setNotifyHandler(notify_handler_t handler);
    void setServiceChangedHandler(service_changed_handler_t handler);

    void addService(ServDescPtr newService);
    int removeService(std::string name);
    int removeService(const CServiceDesc& rmSd );
    int hasService(std::string name);

    void notifyOn(std::string service, std::string host, std::string domain);

protected:
	std::vector<ServDescPtr> m_services;
	boost::asio::io_service& m_io_service;


private:
	bool m_notify;
	std::string m_notify_service;
	std::string m_notify_host;
	std::string m_notify_domain;

	notify_handler_t m_notify_handler;
	service_changed_handler_t m_service_changed_handler;

	std::mutex m_mutex;
};

}
#endif /* IDNSSD_H_ */
