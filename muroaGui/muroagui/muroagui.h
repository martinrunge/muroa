#ifndef MUROAGUI_H
#define MUROAGUI_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include "ui_muroagui.h"
#include "cconnection.h"


class muroagui : public QMainWindow
{
    Q_OBJECT

public:
    muroagui(QWidget *parent = 0);
    ~muroagui();

public slots:
    void openConnection();

    void connectionStatusChanged(QString status);


private:
    Ui::muroaguiClass ui;
    CConnection m_connection;

    QLabel m_connection_status_label;

};

#endif // MUROAGUI_H
