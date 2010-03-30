/*
 * CDnsSdAvahiViaQtDBus.cpp
 *
 *  Created on: 21 Mar 2010
 *      Author: martin
 */

#include "CDnsSdAvahiViaQtDBus.h"

#include <QDBusObjectPath>
#include <QDebug>
#include <avahi-common/defs.h>
#include <avahi-common/address.h>

#include "CServiceDesc.h"


CDnsSdAvahiViaQtDBus::CDnsSdAvahiViaQtDBus() : m_dbusConn(QDBusConnection::systemBus()), m_serviceBrowser(0)
{
	m_if = new QDBusInterface("org.freedesktop.Avahi", "/", "org.freedesktop.Avahi.Server", m_dbusConn );

	QList<QVariant> arglist;
	arglist.append(AVAHI_IF_UNSPEC);
	arglist.append(AVAHI_PROTO_UNSPEC);
	arglist.append(QString("_muroa._tcp"));
	arglist.append(QString("local"));
	arglist.append(unsigned(0));

	QDBusPendingCall pcall = m_if->asyncCallWithArgumentList("ServiceBrowserNew", arglist);
	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
	QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(gotServiceBrowser(QDBusPendingCallWatcher*)));

	resolveService(AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, "muroa", "_muroa._tcp", "local", unsigned(0));
}


CDnsSdAvahiViaQtDBus::~CDnsSdAvahiViaQtDBus() {
	delete m_if;
}


void CDnsSdAvahiViaQtDBus::gotServiceBrowser(QDBusPendingCallWatcher* result)
{
	qDebug() << QString("gotServiceBrowser");

	QDBusPendingReply<QDBusObjectPath> reply = *result;
    if (reply.isError()) {
    	qDebug() << QString("Error in async reply.");
    } else {
    	bool ok;
    	QString path = reply.argumentAt<0>().path();
    	qDebug() << QString("Path= %1").arg(path);
    	if( m_serviceBrowser == 0)
    	{
    		m_serviceBrowser = new QDBusInterface("org.freedesktop.Avahi", path, "org.freedesktop.Avahi.ServiceBrowser", m_dbusConn);

    		connect(m_serviceBrowser, SIGNAL(ItemNew(int, int, QString, QString, QString, unsigned)),
    				this, SLOT(newItemHandler(int, int, QString, QString, QString, unsigned)));

    		connect(m_serviceBrowser, SIGNAL(ItemRemove(int, int, QString, QString, QString, unsigned)),
    				this, SLOT(delItemHandler(int, int, QString, QString, QString, unsigned)));

    	}
    }
}


void CDnsSdAvahiViaQtDBus::newItemHandler(int interface, int protocol, QString name, QString stype, QString domain, unsigned flags)
{
	qDebug() << QString("newItemHandler if=%1 prot=%2 name=%3 stype=%4 domain=%5 flags=%6").arg(interface).arg(protocol).arg(name).arg(stype).arg(domain).arg(flags);
	resolveService(interface, protocol, name, stype, domain, flags);
}

void CDnsSdAvahiViaQtDBus::delItemHandler(int interface, int protocol, QString name, QString stype, QString domain, unsigned flags)
{
	qDebug() << QString("delItemHandler if=%1 prot=%2 name=%3 stype=%4 domain=%5 flags=%6").arg(interface).arg(protocol).arg(name).arg(stype).arg(domain).arg(flags);
	removeService(name);
	emit servicesChanged();
	emit serviceRemoved(name, domain);
}


void CDnsSdAvahiViaQtDBus::resolveService(int interface, int protocol, QString name, QString stype, QString domain, unsigned flags)
{
	QList<QVariant> resolveServiceArgs;
	resolveServiceArgs.append(interface);
	resolveServiceArgs.append(protocol);
	resolveServiceArgs.append(name);
	resolveServiceArgs.append(stype);
	resolveServiceArgs.append(domain);
	resolveServiceArgs.append(0);
	resolveServiceArgs.append(flags);     // port nr

	QDBusMessage msg = m_if->callWithArgumentList( QDBus::Block, "ResolveService", resolveServiceArgs);

	if(msg.type() == QDBusMessage::ErrorMessage)
	{
		qDebug() << QString("Error calling ResolveService: %1 %2").arg(msg.errorName()).arg(msg.errorMessage());
	}
	else
	{
		qDebug() << QString("ResolveService: %1 %2").arg(msg.signature()).arg(msg.errorMessage());
		QList<QVariant> args = msg.arguments();

		int retinterface = args.at(0).toInt();
		int retprotocol = args.at(1).toInt();
		QString retname = args.at(2).toString();
		QString rettype = args.at(3).toString();
		QString domainName = args.at(4).toString();
		QString hostName = args.at(5).toString();
		int retaprotocol = args.at(6).toInt();
		QString retaddress = args.at(7).toString();
		int portNr = args.at(8).toUInt();

		qDebug() << QString("Service resolved if=%1 prot=%2 name=%3 type=%4 domain=%5 host= %6 addr=%7 port=%8").arg(retinterface).arg(retprotocol).arg(retname).arg(rettype).arg(domainName).arg(hostName).arg(retaddress).arg(portNr);

		addService(new CServiceDesc(name, hostName, domainName, portNr));
		emit servicesChanged();
		emit serviceFound(name, hostName, domainName, portNr);
	}
}

