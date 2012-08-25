/*
 * CServiceDesc.h
 *
 *  Created on: 28 Mar 2010
 *      Author: martin
 */

#ifndef CSERVICEDESC_H_
#define CSERVICEDESC_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace muroa {

class CServiceDesc {
public:
	CServiceDesc();
	CServiceDesc(std::string serviceName, std::string hostName,	std::string domainName, std::string serviceType = std::string(), int portNr = 0, int interface = 0, int protocol = 0);
	virtual ~CServiceDesc();
    int getInterface() const
    {
        return m_interface;
    }

    int getProtocol() const
    {
        return m_protocol;
    }

    void setInterface(int m_interface)
    {
        this->m_interface = m_interface;
    }

    void setProtocol(int m_protocol)
    {
        this->m_protocol = m_protocol;
    }

    std::string getDomainName() const
    {
        return m_domainName;
    }

    std::string getHostName() const
    {
        return m_hostName;
    }

    int getPortNr() const
    {
        return m_portNr;
    }

    std::string getServiceName() const
    {
        return m_serviceName;
    }

    std::string getServiceType() const
    {
        return m_serviceType;
    }

    void setDomainName(std::string m_domainName)
    {
        this->m_domainName = m_domainName;
    }

    void setHostName(std::string m_hostName)
    {
        this->m_hostName = m_hostName;
    }

    void setPortNr(int m_portNr)
    {
        this->m_portNr = m_portNr;
    }

    void setServiceName(std::string m_serviceName)
    {
        this->m_serviceName = m_serviceName;
    }

    void setServiceType(std::string m_serviceType)
    {
        this->m_serviceType = m_serviceType;
    }

    inline bool operator ==(const CServiceDesc &other)
	{
    	if(m_serviceName.compare( other.getServiceName() ) == 0 &&
           m_serviceType.compare( other.getServiceType() ) == 0 &&
    	   m_domainName.compare( other.getDomainName() ) == 0 &&
    	   m_hostName.compare( other.getHostName() ) == 0 &&
    	   m_portNr      == other.getPortNr() &&
    	   m_protocol    == other.getProtocol() &&
    	   m_interface   == other.getInterface())
    	{
    		return true;
    	}
    	else
    		return false;
	}

private:
    std::string m_serviceName;
    std::string m_serviceType;
    std::string m_hostName;
    std::string m_domainName;
	int m_portNr;

	int m_protocol;
	int m_interface;

};

typedef boost::shared_ptr<CServiceDesc> ServDescPtr ;

}

#endif /* CSERVICEDESC_H_ */
