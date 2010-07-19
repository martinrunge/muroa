#include "CMuroaGui.h"
#include "CServiceBrowser.h"
#include "CPreferencesDlg.h"


CMuroaGui::CMuroaGui(QWidget *parent)
    : QMainWindow(parent) ,
      m_dnssd("_muroa._tcp"),
   	  m_diffBuilder(&m_collection, &m_playlist, &m_nextlist)
{
	ui.setupUi(this);

	ui.playBtn->setDefaultAction(ui.actionPlayPause);
	ui.stopBtn->setDefaultAction(ui.actionStop);
	ui.nextBtn->setDefaultAction(ui.actionNext);
	ui.prevBtn->setDefaultAction(ui.actionPrevious);


	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    connect(ui.actionClose, SIGNAL(triggered()), &m_connection, SLOT(close()));

    connect(ui.actionGet_Revision_1, SIGNAL(triggered()), &m_connection, SLOT(close()));
    connect(ui.actionGet_next_Revision, SIGNAL(triggered()), &m_connection, SLOT(close()));

    connect(ui.actionPlayPause, SIGNAL(triggered()), &m_connection, SLOT(play()));
    connect(ui.actionStop, SIGNAL(triggered()), &m_connection, SLOT(stop()));
    connect(ui.actionNext, SIGNAL(triggered()), &m_connection, SLOT(next()));
    connect(ui.actionPrevious, SIGNAL(triggered()), &m_connection, SLOT(prev()));

    connect(ui.action_Preferences, SIGNAL(triggered()), this, SLOT(showPreferences()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(enum connectionState)), this, SLOT(connectionStatusChanged(enum connectionState)));
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
	setupServiceBrowser();

	connect(&m_connection, SIGNAL(progressSig(int, int)), this, SLOT(progress(int,int)));
	connectionStatusChanged( e_disconnected );
}

CMuroaGui::~CMuroaGui()
{
	delete m_serviceBrowser;
}

void CMuroaGui::showPreferences()
{
	CPreferencesDlg prefs;
	prefs.exec();
}

void CMuroaGui::openConnection() {
	int index = m_serviceBrowser->exec();
	CServiceDesc* sd = m_dnssd.getService(index);

	openConnection(*sd);
}

void CMuroaGui::openConnection(const CServiceDesc &sd) {
    m_connection.open(sd.getHostName(), sd.getPortNr());

	QSettings settings;
	settings.setValue("rejoin_service", sd.getServiceName());
	settings.setValue("rejoin_host", sd.getHostName());
	settings.setValue("rejoin_domain", sd.getDomainName());

}


void CMuroaGui::connectionStatusChanged(enum connectionState status) {
	QString statusMsg;
	if( status == e_connected ) {
		statusMsg = QString("Connected.");
		ui.actionPlayPause->setEnabled( true );
		ui.actionStop->setEnabled( true );
		ui.actionClose->setEnabled(true);
		ui.actionOpen->setEnabled( false );
	}
	else {
		statusMsg = QString("Not Connected.");
		ui.actionPlayPause->setEnabled( false );
		ui.actionStop->setEnabled( false );
		ui.actionClose->setEnabled(false);
		ui.actionOpen->setEnabled( true );
	}
	m_connection_status_label.setText(statusMsg);
}

void CMuroaGui::progress(int done, int total)
{
	int done_min = done / 60;
	int done_sec = done - done_min * 60;

	int total_min = total / 60;
	int total_sec = total - total_min * 60;

	QString progLabel = QString("%1:%2 / %3:%4").arg(done_min).arg(done_sec, 2, 10, QChar('0')).arg(total_min).arg(total_sec, 2, 10, QChar('0'));
	ui.progressLabel->setText( progLabel );

	ui.posSlider->setValue((done * 100) / total);

	if(done == 0) {
		CPlaylistItem* plItem = m_nextlist.at(0);
		CCollectionItem* colItem = m_collection.getByHash( plItem->getHash() );
		ui.titleLabel->setText( QString("%1 - %2").arg(colItem->getArtist()).arg(colItem->getTitle()) );
	}
}


void CMuroaGui::setupServiceBrowser()
{
	m_serviceBrowser = new CServiceBrowser(&m_dnssd);
	connect(&m_dnssd, SIGNAL(servicesChanged()), m_serviceBrowser, SLOT(servicesChanged()));

	QSettings settings;
	bool rejoin = settings.value("rejoin").toBool();
	if( rejoin ) {

		QString service = settings.value("rejoin_service").toString();
		QString host = settings.value("rejoin_host").toString();
		QString domain = settings.value("rejoin_domain").toString();

		m_dnssd.notifyOn( service, host, domain );
	}
	connect(&m_dnssd, SIGNAL(notifyService(const CServiceDesc&)), this, SLOT(openConnection(const CServiceDesc&)));
}
