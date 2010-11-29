/*
 * CDnsSdAvahiViaQtDBus.h
 *
 *  Created on: 21 Mar 2010
 *      Author: martin
 */

#ifndef CDNSSD_H_
#define CDNSSD_H_

#include <QObject>
#include <QtDBus/QtDBus>


class CDnsSdAvahiViaQtDBus : public QObject {
	Q_OBJECT;
public:
	CDnsSdAvahiViaQtDBus();
	virtual ~CDnsSdAvahiViaQtDBus();

public slots:
	void stateChanged(int state);
	void gotState(QDBusPendingCallWatcher* result);
	void gotEntryGroup(QDBusPendingCallWatcher* result);


private:
	void prepareAddService(void);
	void addService(void);
	void removeService(void);

	QDBusConnection m_dbusConn;
	QDBusInterface *m_if;
	QDBusInterface *m_entryGroup;

};

#endif /* CDNSSD_H_ */
