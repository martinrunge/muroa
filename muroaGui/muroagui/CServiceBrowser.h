/*
 * CServiceBrowser.h
 *
 *  Created on: 27 Mar 2010
 *      Author: martin
 */

#ifndef CSERVICEBROWSER_H_
#define CSERVICEBROWSER_H_

#include <QDialog>
#include <QStringListModel>

#include "ui_ServiceBrowser.h"

class CServiceBrowser : public QDialog, public Ui::ServiceBrowserDialog {
	Q_OBJECT;
public:
	CServiceBrowser();
	virtual ~CServiceBrowser();

	void accept();
	void reject();

signals:
	void serviceSelected(int index);

public slots:
	void addService(QString service, QString host, QString domain, int port);
	void removeService(QString service, QString domain);
	void removeService(int index);

private:
	QStringListModel m_model;
	QStringList m_serviceList;
};

#endif /* CSERVICEBROWSER_H_ */
