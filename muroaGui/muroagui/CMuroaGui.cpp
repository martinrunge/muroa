#include "CMuroaGui.h"
#include "CServiceBrowser.h"


CMuroaGui::CMuroaGui(QWidget *parent)
    : QMainWindow(parent) ,
      m_dnssd("_muroa._tcp"),
   	  m_diffBuilder(&m_collection, &m_playlist, &m_nextlist)
{
	ui.setupUi(this);

	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    connect(ui.actionClose, SIGNAL(triggered()), &m_connection, SLOT(close()));

    connect(ui.actionGet_Revision_1, SIGNAL(triggered()), &m_connection, SLOT(close()));
    connect(ui.actionGet_next_Revision, SIGNAL(triggered()), &m_connection, SLOT(close()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));
	m_connection.setNextlistModelPtr(&m_nextlistModel);
	m_connection.setPlaylistModelPtr(&m_playlistModel);
	m_connection.setColletionModelPtr(&m_collectionModel);

	m_nextlistModel.setCollections(&m_nextlist, &m_collection);
	m_playlistModel.setCollections(&m_playlist, &m_collection);
	m_collectionModel.setCollection(&m_collection);

	ui.collectionView->setModel(&m_collectionModel);
	ui.playlistView->setModel(&m_playlistModel);
	ui.nextToPlayView->setModel(&m_nextlistModel);

	ui.collectionView->setDiffBuilderPtr(&m_diffBuilder);
	ui.playlistView->setDiffBuilderPtr(&m_diffBuilder);
	ui.nextToPlayView->setDiffBuilderPtr(&m_diffBuilder);
	ui.nextToPlayView->setRole(E_NEXTLIST);

	connect(&m_diffBuilder, SIGNAL(sendCommand(CCommandBase*)), &m_connection, SLOT(sendCommand(CCommandBase*)));
	m_serviceBrowser = new CServiceBrowser(&m_dnssd);
	connect(&m_dnssd, SIGNAL(servicesChanged()), m_serviceBrowser, SLOT(servicesChanged()));
	// m_serviceBrowser->exec();
}

CMuroaGui::~CMuroaGui()
{
	delete m_serviceBrowser;
}

void CMuroaGui::openConnection()
{
	int index = m_serviceBrowser->exec();
	CServiceDesc* sd = m_dnssd.getService(index);

    m_connection.open(sd->getHostName(), sd->getPortNr());
}

void CMuroaGui::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}

