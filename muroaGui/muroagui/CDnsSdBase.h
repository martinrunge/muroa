/*
 * CDnsSdBase.h
 *
 *  Created on: 25 Mar 2010
 *      Author: martin
 */

#ifndef IDNSSD_H_
#define IDNSSD_H_

#include <QList>
#include <QObject>

class CServiceDesc;

class CDnsSdBase: public QObject {
	Q_OBJECT;
public:
	CDnsSdBase();
	virtual ~CDnsSdBase();

    QList<CServiceDesc*> getServiceList() const
    {
        return m_serviceList;
    }

    void setServiceList(QList<CServiceDesc*> m_serviceList)
    {
        this->m_serviceList = m_serviceList;
    }

    void addService(CServiceDesc* newService);
    int removeService(QString name);
    int hasService(QString name);


protected:
	QList<CServiceDesc*> m_serviceList;
};

#endif /* IDNSSD_H_ */
