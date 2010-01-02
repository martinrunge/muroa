#include "muroagui.h"

muroagui::muroagui(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    connect(ui.actionClose, SIGNAL(triggered()), &m_connection, SLOT(close()));

    connect(ui.actionGet_Revision_1, SIGNAL(triggered()), &m_connection, SLOT(close()));
    connect(ui.actionGet_next_Revision, SIGNAL(triggered()), &m_connection, SLOT(close()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));
	m_connection.setPlaylistModelPtr(&m_playlistModel);
	m_connection.setColletionModelPtr(&m_collectionModel);

	//m_playlistModel.setPlaylist(&m_playlist);
	m_playlistModel.setCollections(&m_playlist, &m_collection);
	m_collectionModel.setCollection(&m_collection);

	ui.playlistView->setModel(&m_playlistModel);
	ui.collectionView->setModel(&m_collectionModel);

}

muroagui::~muroagui()
{

}



void muroagui::openConnection()
{
    m_connection.open("localhost", 2678);
}


void muroagui::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}
