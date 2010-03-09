#ifndef MUROAGUI_H
#define MUROAGUI_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include "CCollection.h"

#include "CDiffBuilder.h"

#include "CPlaylistModel.h"
#include "CCollectionModel.h"
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

    CCollection<CCollectionItem*> m_collection;
    CCollection<CPlaylistItem*> m_playlist;
    CCollection<CPlaylistItem*> m_playnext;

    CDiffBuilder m_diffBuilder;

    CCollectionModel m_collectionModel;
    CPlaylistModel m_playlistModel;
    CPlaylistModel m_playnextModel;

    QLabel m_connection_status_label;

};

#endif // MUROAGUI_H
