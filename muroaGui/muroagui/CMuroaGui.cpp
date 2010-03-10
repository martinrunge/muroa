#include "CMuroaGui.h"

CMuroaGui::CMuroaGui(QWidget *parent)
    : QMainWindow(parent) ,
   	  m_diffBuilder(&m_collection, &m_playlist, &m_playnext)
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
	m_playnextModel.setCollections(&m_playnext, &m_collection);
	m_collectionModel.setCollection(&m_collection);

	//ui.playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.playlistView->setDragEnabled(true);
	ui.playlistView->viewport()->setAcceptDrops(true);
	ui.playlistView->setDropIndicatorShown(true);
	// ui.playlistView->setDragDropMode(QAbstractItemView::InternalMove);
	ui.playlistView->setDragDropOverwriteMode( false );

	ui.collectionView->setModel(&m_collectionModel);
	ui.playlistView->setModel(&m_playlistModel);
	ui.nextToPlayView->setModel(&m_playnextModel);

	ui.collectionView->setDiffBuilderPtr(&m_diffBuilder);
	ui.playlistView->setDiffBuilderPtr(&m_diffBuilder);
	ui.nextToPlayView->setDiffBuilderPtr(&m_diffBuilder);
	ui.nextToPlayView->setRole(E_NEXTLIST);

	connect(&m_diffBuilder, SIGNAL(sendCommand(CCommandBase*)), &m_connection, SLOT(sendCommand(CCommandBase*)));
}

CMuroaGui::~CMuroaGui()
{

}



void CMuroaGui::openConnection()
{
    m_connection.open("localhost", 2678);
}


void CMuroaGui::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}
