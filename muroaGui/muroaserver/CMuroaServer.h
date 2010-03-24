#ifndef MUROASERVER_H
#define MUROASERVER_H

#include <QtGui/QMainWindow>
#include "ui_muroaserver.h"
#include "CConnection.h"
#include "CCollection.h"
#include "CCollectionItem.h"
#include "CDnsSdAvahiViaQtDBus.h"
#include "CDnsSdAvahi.h"


class CSession;
class CNetwork;

class CMuroaServer : public QMainWindow
{
    Q_OBJECT;

public:
    CMuroaServer(QWidget *parent = 0);
    ~CMuroaServer();

    void readCollectionFile(QString filename);


public slots:
	void newConnection(QTcpSocket* socket);
	void connectionStatusChanged(QString status);
	void nextRevision();

private:
	CSession *m_session;

	CNetwork* m_net;

    Ui::muroaserverClass ui;

    // CConnection m_connection;
    CCollection<CCollectionItem> m_collection;

    QLabel m_connection_status_label;

    QStringList m_testfiles;

    //CDnsSdAvahiViaQtDBus m_dnssd;
    CDnsSdAvahi m_dnssd;

};

#endif // MUROASERVER_H
