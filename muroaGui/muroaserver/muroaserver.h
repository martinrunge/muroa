#ifndef MUROASERVER_H
#define MUROASERVER_H

#include <QtGui/QMainWindow>
#include "ui_muroaserver.h"
#include "cconnection.h"
#include "CCollection.h"
#include "CCollectionItem.h"

class CSession;
class CNetwork;

class muroaserver : public QMainWindow
{
    Q_OBJECT;

public:
    muroaserver(QWidget *parent = 0);
    ~muroaserver();

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

};

#endif // MUROASERVER_H
