#ifndef MUROASERVER_H
#define MUROASERVER_H

#include <QtGui/QMainWindow>
#include "ui_muroaserver.h"
#include "cconnection.h"

class muroaserver : public QMainWindow
{
    Q_OBJECT

public:
    muroaserver(QWidget *parent = 0);
    ~muroaserver();

public slots:
	void connectionStatusChanged(QString status);



private:
    Ui::muroaserverClass ui;

    CConnection m_connection;

    QLabel m_connection_status_label;

};

#endif // MUROASERVER_H
