#ifndef MUROASERVER_H
#define MUROASERVER_H

#include <QtGui/QMainWindow>
#include "ui_MuroaServer.h"
#include "CConnection.h"
#include "CCollection.h"
#include "CCollectionItem.h"
#include "CDnsSdAvahiViaQtDBus.h"
#include "CDnsSdAvahi.h"

#include "CMediaScannerCtrl.h"

#include "mediaprocessing/CCollectionUpdater.h"


class CSession;
class CNetwork;

class CMuroaServer : public QMainWindow
{
    Q_OBJECT;

public:
    CMuroaServer(QWidget *parent = 0);
    ~CMuroaServer();

    void addCollectionRev(CCollection<CCollectionItem>* collection);

    void readCollectionFile(QString filename);


public slots:
	void newConnection(QTcpSocket* socket);
	void connectionStatusChanged(QString status);
	void nextRevision();
	void scanCollection();

private:
	CSession *m_session;
	CNetwork* m_net;
	CStateDB* m_stateDB;

	QString m_db_filename;

	CMediaScannerCtrl m_mediaScannerCtrl;

	void readSettings();
	void writeSettings();

	QString m_mediadir;
	CCollectionUpdater m_collectionUpdater;

    Ui::muroaserverClass ui;
    QLabel m_connection_status_label;

    // CConnection m_connection;
    CCollection<CCollectionItem> m_collection;

    //CDnsSdAvahiViaQtDBus m_dnssd;
    CDnsSdAvahi m_dnssd;
    int m_portNr;
};

#endif // MUROASERVER_H
