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
	virtual ~CServiceDesc();

    QString getDomainName() const
    {
        return m_DomainName;
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

    void setDomainName(QString m_DomainName)
    {
        this->m_DomainName = m_DomainName;
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

private:
	QString m_serviceName;
	QString m_hostName;
	QString m_DomainName;
	int m_portNr;

};

#endif /* CSERVICEDESC_H_ */
