#ifndef MUROAGUI_H
#define MUROAGUI_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include "CCollection.h"

#include "CDiffBuilder.h"

#include "CPlaylistModel.h"
#include "CCollectionModel.h"
#include "ui_MuroaGui.h"
#include "CConnection.h"

#include "CDnsSdAvahiViaQtDBus.h"
#include "CDnsSdServiceBrowserAvahi.h"

class CServiceBrowser;

class CMuroaGui : public QMainWindow
{
    Q_OBJECT

public:
    CMuroaGui(QWidget *parent = 0);
    ~CMuroaGui();

public slots:
    void openConnection();
    void connectionStatusChanged(QString status);

    void progress(int done, int total);
    // void setCurrentTitle( );


private:
    Ui::muroaguiClass ui;
    CConnection m_connection;

    CServiceBrowser* m_serviceBrowser;
//    CDnsSdAvahiViaQtDBus m_dnssd;
    CDnsSdServiceBrowserAvahi m_dnssd;

    CCollection<CCollectionItem*> m_collection;
    CCollection<CPlaylistItem*> m_playlist;
    CCollection<CPlaylistItem*> m_nextlist;

    CDiffBuilder m_diffBuilder;

    CCollectionModel m_collectionModel;
    CPlaylistModel m_playlistModel;
    CPlaylistModel m_nextlistModel;

    QLabel m_connection_status_label;

};

#endif // MUROAGUI_H
