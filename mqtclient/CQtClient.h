#ifndef MUROAGUI_H
#define MUROAGUI_H

#include <QtGui/QMainWindow>
#include <QLabel>

#include "ui_mQtClient.h"

#include "CDnsSdServiceBrowserAvahi.h"
#include "CSession.h"
#include "CSessionAdminDlg.h"

class CDiffBuilder;

class CServiceBrowser;

class CQtClient : public QMainWindow
{
    Q_OBJECT

public:
    CQtClient(QWidget *parent = 0);
    ~CQtClient();

public slots:
	void showPreferences();
	void showSessionAdmin();
    void openConnection();
    void connectionStatusChanged(enum connectionState status);
    void progress(int done, int total);
    // void setCurrentTitle( );

private:
    Ui::muroaguiClass ui;

    CSession m_session;
    CDiffBuilder* m_diffBuilder;

    CSessionAdminDlg m_sessionAdminDlg;

    void setupServiceBrowser();
    CServiceBrowser* m_serviceBrowser;
    CDnsSdServiceBrowserAvahi m_dnssd;

    QLabel m_connection_status_label;

};

#endif // MUROAGUI_H
