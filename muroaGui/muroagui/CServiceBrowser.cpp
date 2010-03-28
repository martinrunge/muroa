/*
 * CServiceBrowser.cpp
 *
 *  Created on: 27 Mar 2010
 *      Author: martin
 */

#include "CServiceBrowser.h"

#include <QDebug>

CServiceBrowser::CServiceBrowser() {
	setupUi(this);

	m_model.setStringList(m_serviceList);
	listView->setModel(&m_model);
}

CServiceBrowser::~CServiceBrowser() {

}

void CServiceBrowser::accept()
{
	int index;
	emit serviceSelected(index);
	hide();
}

void CServiceBrowser::reject()
{
	hide();
}



void CServiceBrowser::addService(QString service, QString host, QString domain, int port)
{
	qDebug() << QString("CServiceBrowser::addService %1 %2 %3").arg(service).arg(host).arg(port);
	m_serviceList.append(service);
	m_model.setStringList(m_serviceList);
}

void CServiceBrowser::removeService(QString service, QString domain)
{
	qDebug() << QString("CServiceBrowser::removeService %1 %2").arg(service).arg(domain);
	int numremoved = m_serviceList.removeAll(service);
	m_model.setStringList(m_serviceList);
}

void CServiceBrowser::removeService(int index)
{
	m_serviceList.removeAt(index);
	m_model.setStringList(m_serviceList);
}



