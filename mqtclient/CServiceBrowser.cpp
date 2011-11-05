/*
 * CServiceBrowser.cpp
 *
 *  Created on: 27 Mar 2010
 *      Author: martin
 */

#include "CServiceBrowser.h"

#include "CDnsSdBase.h"
#include "CServiceDesc.h"
#include <QDebug>

CServiceBrowser::CServiceBrowser(CDnsSdBase* dnssdObj) {
	setupUi(this);
	m_dnssdObj = dnssdObj;
	m_selected_index = -1;

	setupServiceList();

	m_model.setStringList(m_serviceList);
	listView->setModel(&m_model);
}

CServiceBrowser::~CServiceBrowser() {

}

void CServiceBrowser::accept()
{
	QItemSelectionModel *selectionModel = listView->selectionModel();
	QModelIndexList idxlist = selectionModel->selectedIndexes();

	if(idxlist.size() > 0) {
		m_selected_index = idxlist.at(0).row();
	}
	else {
		m_selected_index = -1;
	}
	//hide();
	QDialog::accept();
}

void CServiceBrowser::reject()
{
	m_selected_index = -1;
	//hide();
	QDialog::reject();
}

int CServiceBrowser::getSelectedIndex() {
	return m_selected_index;
}

void CServiceBrowser::servicesChanged()
{
	setupServiceList();
}


void CServiceBrowser::setupServiceList()
{
	m_serviceList.clear();
	QList<CServiceDesc*> services = m_dnssdObj->getServiceList();
	for(int i=0; i < services.size(); i++)
	{
		QString serviceName = QString("%1    (%2)").arg(services.at(i)->getServiceName()).arg(services.at(i)->getHostName());
		m_serviceList.append(serviceName);
	}
	m_model.setStringList(m_serviceList);
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



