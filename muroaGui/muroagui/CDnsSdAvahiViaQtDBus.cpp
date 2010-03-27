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
	qDebug() << QString("delItemHandler");
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
		QString retdomain = args.at(4).toString();
		QString rethost = args.at(5).toString();
		int retaprotocol = args.at(6).toInt();
		QString retaddress = args.at(7).toString();
		int retport = args.at(8).toUInt();

		qDebug() << QString("Service resolved if=%1 prot=%2 name=%3 type=%4 domain=%5 addr=%6 port=%7").arg(retinterface).arg(retprotocol).arg(retname).arg(rettype).arg(retdomain).arg(retaddress).arg(retport);

	}
}


//void CDnsSdAvahiViaQtDBus::prepareAddService(void)
//{
//	if(m_entryGroup == 0)
//	{
//		QDBusPendingCall pcall = m_if->asyncCall("EntryGroupNew");
//		QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
//		QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(gotEntryGroup(QDBusPendingCallWatcher*)));
//	}
//	else
//	{
//		addService();
//	}
//}
//
//void CDnsSdAvahiViaQtDBus::addService(void)
//{
//	QList<QVariant> addServiceArgs;
//	addServiceArgs.append(AVAHI_IF_UNSPEC);
//	addServiceArgs.append(AVAHI_PROTO_UNSPEC);
//	addServiceArgs.append(unsigned(0));
//	addServiceArgs.append("Muroa session server");
//	addServiceArgs.append("_muroa_sessions._tcp");
//	addServiceArgs.append(QString());
//	addServiceArgs.append(QString());
//	addServiceArgs.append(ushort(5678));     // port nr
//	//addServiceArgs.append(QString("test-blah").toAscii());
//	//addServiceArgs.append(QString("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678").toAscii());
//
//	QDBusArgument dBusArg;
//	//dBusArg << AVAHI_IF_UNSPEC;
//	//dBusArg << AVAHI_PROTO_UNSPEC;
//	//dBusArg << unsigned(0);
//	//dBusArg << "Muroa session server";
//	//dBusArg << "_muroa_sessions._tcp";
//	//dBusArg << QString();
//	//dBusArg << QString();
////	dBusArg << (ushort(5678));     // port nr
//	dBusArg.beginArray(qMetaTypeId<QByteArray>());
//	dBusArg << QString("test-blah").toAscii();
//	dBusArg << QString("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678").toAscii();
//	dBusArg.endArray();
//
//	// addServiceArgs.append(dBusArg.asVariant());
//
//	QList<QVariant> txtRec;
//	txtRec.append(QString("test-blah").toAscii());
//	txtRec.append(QString("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678").toAscii());
//
//	addServiceArgs.append(txtRec);
//
//	QDBusMessage msg = m_entryGroup->callWithArgumentList(QDBus::Block, "AddService", addServiceArgs);
//	//QDBusMessage msg = m_entryGroup->call("AddService", dBusArg.asVariant());
//
//	if(msg.type() == QDBusMessage::ErrorMessage)
//	{
//		qDebug() << QString("Error calling AddService: %1 %2").arg(msg.errorName()).arg(msg.errorMessage());
//	}
////	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
////	QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(gotEntryGroup(QDBusPendingCallWatcher*)));
//
//	msg = m_entryGroup->call("Commit");
//	if(msg.type() == QDBusMessage::ErrorMessage)
//	{
//		qDebug() << QString("Error calling AddService: %1 %2").arg(msg.errorName()).arg(msg.errorMessage());
//	}
//    // group.Commit()
//
//}
//
//void CDnsSdAvahiViaQtDBus::removeService(void)
//{
//
//}
//
//
//void CDnsSdAvahiViaQtDBus::gotEntryGroup(QDBusPendingCallWatcher* result)
//{
//	qDebug() << QString("gotState");
//
//	QDBusPendingReply<QDBusObjectPath> reply = *result;
//    if (reply.isError()) {
//    	qDebug() << QString("Error in async reply to EntryGroupNew().");
//    } else {
//    	QDBusObjectPath path = reply.argumentAt<0>();
//    	qDebug() << QString("New Entry Group's object path: %1").arg(path.path());
//
//    	m_entryGroup = new QDBusInterface("org.freedesktop.Avahi", path.path(), "org.freedesktop.Avahi.EntryGroup", m_dbusConn );
//        addService();
//    }
//}
