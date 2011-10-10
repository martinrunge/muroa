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


CDnsSdAvahiViaQtDBus::CDnsSdAvahiViaQtDBus() : m_dbusConn(QDBusConnection::systemBus()), m_entryGroup(0)
{
	m_if = new QDBusInterface("org.freedesktop.Avahi", "/", "org.freedesktop.Avahi.Server", m_dbusConn );
	connect(m_if, SIGNAL(StateChanged(int)), this, SLOT(stateChanged(int)));

	QDBusPendingCall pcall = m_if->asyncCall("GetState");
	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
	QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(gotState(QDBusPendingCallWatcher*)));
}


CDnsSdAvahiViaQtDBus::~CDnsSdAvahiViaQtDBus() {
	delete m_if;
}



void CDnsSdAvahiViaQtDBus::stateChanged(int state)
{
	switch(state)
	{
	case AVAHI_SERVER_COLLISION:
        qDebug() << QString("WARNING: Server name collision");
        removeService();
        break;
	case AVAHI_SERVER_RUNNING:
        prepareAddService();
        break;
	default:
		break;
	}
}


void CDnsSdAvahiViaQtDBus::gotState(QDBusPendingCallWatcher* result)
{
	qDebug() << QString("gotState");

	QDBusPendingReply<int> reply = *result;
    if (reply.isError()) {
    	qDebug() << QString("Error in async reply.");
    } else {
    	bool ok;
    	int state = reply.argumentAt(0).toInt(&ok);
    	qDebug() << QString("State= %1").arg(state);
        stateChanged(state);
    }
}


void CDnsSdAvahiViaQtDBus::prepareAddService(void)
{
	if(m_entryGroup == 0)
	{
		QDBusPendingCall pcall = m_if->asyncCall("EntryGroupNew");
		QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
		QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(gotEntryGroup(QDBusPendingCallWatcher*)));
	}
	else
	{
		addService();
	}
}

void CDnsSdAvahiViaQtDBus::addService(void)
{
	QList<QVariant> addServiceArgs;
	addServiceArgs.append(AVAHI_IF_UNSPEC);
	addServiceArgs.append(AVAHI_PROTO_UNSPEC);
	addServiceArgs.append(unsigned(0));
	addServiceArgs.append("Muroa session server");
	addServiceArgs.append("_muroa_sessions._tcp");
	addServiceArgs.append(QString());
	addServiceArgs.append(QString());
	addServiceArgs.append(ushort(5678));     // port nr
	//addServiceArgs.append(QString("test-blah").toAscii());
	//addServiceArgs.append(QString("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678").toAscii());

	QDBusArgument dBusArg;
	//dBusArg << AVAHI_IF_UNSPEC;
	//dBusArg << AVAHI_PROTO_UNSPEC;
	//dBusArg << unsigned(0);
	//dBusArg << "Muroa session server";
	//dBusArg << "_muroa_sessions._tcp";
	//dBusArg << QString();
	//dBusArg << QString();
//	dBusArg << (ushort(5678));     // port nr
	dBusArg.beginArray(qMetaTypeId<QByteArray>());
	dBusArg << QString("test-blah").toAscii();
	dBusArg << QString("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678").toAscii();
	dBusArg.endArray();

	// addServiceArgs.append(dBusArg.asVariant());

	QList<QVariant> txtRec;
	txtRec.append(QString("test-blah").toAscii());
	txtRec.append(QString("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678").toAscii());

	addServiceArgs.append(txtRec);

	QDBusMessage msg = m_entryGroup->callWithArgumentList(QDBus::Block, "AddService", addServiceArgs);
	//QDBusMessage msg = m_entryGroup->call("AddService", dBusArg.asVariant());

	if(msg.type() == QDBusMessage::ErrorMessage)
	{
		qDebug() << QString("Error calling AddService: %1 %2").arg(msg.errorName()).arg(msg.errorMessage());
	}
//	QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pcall, this);
//	QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(gotEntryGroup(QDBusPendingCallWatcher*)));

	msg = m_entryGroup->call("Commit");
	if(msg.type() == QDBusMessage::ErrorMessage)
	{
		qDebug() << QString("Error calling AddService: %1 %2").arg(msg.errorName()).arg(msg.errorMessage());
	}
    // group.Commit()

}

void CDnsSdAvahiViaQtDBus::removeService(void)
{

}


void CDnsSdAvahiViaQtDBus::gotEntryGroup(QDBusPendingCallWatcher* result)
{
	qDebug() << QString("gotState");

	QDBusPendingReply<QDBusObjectPath> reply = *result;
    if (reply.isError()) {
    	qDebug() << QString("Error in async reply to EntryGroupNew().");
    } else {
    	QDBusObjectPath path = reply.argumentAt<0>();
    	qDebug() << QString("New Entry Group's object path: %1").arg(path.path());

    	m_entryGroup = new QDBusInterface("org.freedesktop.Avahi", path.path(), "org.freedesktop.Avahi.EntryGroup", m_dbusConn );
        addService();
    }
}
