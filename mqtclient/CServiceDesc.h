/*
 * CServiceDesc.h
 *
 *  Created on: 28 Mar 2010
 *      Author: martin
 */

#ifndef CSERVICEDESC_H_
#define CSERVICEDESC_H_

#include <QString>

class CServiceDesc {
public:
	CServiceDesc();
	CServiceDesc(QString serviceName, QString hostName,	QString domainName, int portNr = 0, int interface = 0, int protocol = 0);
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

    QString getDomainName() const
    {
        return m_domainName;
    }

    QString getHostName() const
    {
        return m_hostName;
    }

    int getPortNr() const
    {
        return m_portNr;
    }

    QString getServiceName() const
    {
        return m_serviceName;
    }

    void setDomainName(QString m_domainName)
    {
        this->m_domainName = m_domainName;
    }

    void setHostName(QString m_hostName)
    {
        this->m_hostName = m_hostName;
    }

    void setPortNr(int m_portNr)
    {
        this->m_portNr = m_portNr;
    }

    void setServiceName(QString m_serviceName)
    {
        this->m_serviceName = m_serviceName;
    }

    inline bool operator ==(const CServiceDesc &other)
	{
    	if(m_serviceName == other.getServiceName() &&
    	   m_domainName  == other.getDomainName() &&
    	   m_protocol    == other.getProtocol() &&
    	   m_interface   == other.getInterface())
    	{
    		return true;
    	}
    	else
    		return false;
	}

private:
	QString m_serviceName;
	QString m_hostName;
	QString m_domainName;
	int m_portNr;

	int m_protocol;
	int m_interface;

};

#endif /* CSERVICEDESC_H_ */
