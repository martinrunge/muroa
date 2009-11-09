#ifndef MUROASERVER_H
#define MUROASERVER_H

#include <QtGui/QMainWindow>
#include "ui_muroaserver.h"
#include "cconnection.h"
#include "CCollection.h"

class CSession;

class muroaserver : public QMainWindow
{
    Q_OBJECT

public:
    muroaserver(QWidget *parent = 0);
    ~muroaserver();

    void readCollectionFile(QString filename);


public slots:
	void connectionStatusChanged(QString status);



private:
	CSession *m_session;

    Ui::muroaserverClass ui;

    CConnection m_connection;
    CCollection m_collection;

    QLabel m_connection_status_label;

};

#endif // MUROASERVER_H
