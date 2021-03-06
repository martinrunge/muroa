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

typedef boost::function<void(void)> service_changed_handler_t;
typedef boost::function<void(ServDescPtr)> add_service_handler_t;
// typedef boost::function<void(std::string name)> rm_service_handler_t;

class CDnsSdBase {
public:
	CDnsSdBase(boost::asio::io_service& io_service);
	virtual ~CDnsSdBase();

    std::vector<ServDescPtr> getServiceList() const;
    void setServiceList(std::vector<ServDescPtr> services);

    ServDescPtr getServiceByName(std::string name);
    ServDescPtr getService(int index) { return m_services.at(index); };

    void setServiceChangedHandler(service_changed_handler_t handler);
    void setServiceAddedHandler(add_service_handler_t handler);
    void setServiceRemovedHandler(add_service_handler_t handler);

    void addService(ServDescPtr newService);
    int removeService(std::string name);
    int removeService(const CServiceDesc& rmSd );
    int hasService(std::string name);

protected:
	std::vector<ServDescPtr> m_services;
	boost::asio::io_service& m_io_service;


private:
	service_changed_handler_t m_service_changed_handler;
	add_service_handler_t m_add_service_handler;
	add_service_handler_t m_rm_service_handler;

	std::mutex m_mutex;

	CApp* m_app;
};

}
#endif /* IDNSSD_H_ */
