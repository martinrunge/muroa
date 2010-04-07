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

#include "CServiceDesc.h"

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

    CServiceDesc* getService(QString name, int which = 0);
    CServiceDesc* getService(int index) { return m_serviceList.at(index); };

    void addService(CServiceDesc* newService);
    int removeService(QString name);
    int removeService(const CServiceDesc& rmSd );
    int hasService(QString name);

signals:
    void servicesChanged();

protected:
	QList<CServiceDesc*> m_serviceList;
};

#endif /* IDNSSD_H_ */
