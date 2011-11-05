#include "CQtClient.h"
#include "CServiceBrowser.h"
#include "CPreferencesDlg.h"


CQtClient::CQtClient(QWidget *parent)
    : QMainWindow(parent) ,
      m_dnssd("_muroa._tcp")
{
	ui.setupUi(this);

	ui.playBtn->setDefaultAction(ui.actionPlayPause);
	ui.stopBtn->setDefaultAction(ui.actionStop);
	ui.nextBtn->setDefaultAction(ui.actionNext);
	ui.prevBtn->setDefaultAction(ui.actionPrevious);


	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(closeConnection()));

//    connect(ui.actionPlayPause, SIGNAL(triggered()), &m_connection, SLOT(play()));
//    connect(ui.actionStop, SIGNAL(triggered()), &m_connection, SLOT(stop()));
//    connect(ui.actionNext, SIGNAL(triggered()), &m_connection, SLOT(next()));
//    connect(ui.actionPrevious, SIGNAL(triggered()), &m_connection, SLOT(prev()));

    connect(ui.action_Preferences, SIGNAL(triggered()), this, SLOT(showPreferences()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(enum connectionState)), this, SLOT(connectionStatusChanged(enum connectionState)));

	setupServiceBrowser();


//	connect(&m_connection, SIGNAL(progressSig(int, int)), this, SLOT(progress(int,int)));
	connectionStatusChanged( e_disconnected );
}

CQtClient::~CQtClient()
{
	delete m_serviceBrowser;
}

void CQtClient::showPreferences()
{
	CPreferencesDlg prefs;
	prefs.exec();
}

void CQtClient::openConnection() {
	int rc = m_serviceBrowser->exec();

	int index = -1;
	if(rc == QDialog::Accepted) {
		index = m_serviceBrowser->getSelectedIndex();
	}

	if(index != -1) {
		CServiceDesc* sd = m_dnssd.getService(index);
		openConnection(*sd);
	}
}

void CQtClient::openConnection(const CServiceDesc &sd) {
    m_connection.open(sd.getHostName(), sd.getPortNr());

	QSettings settings;
	settings.setValue("rejoin_service", sd.getServiceName());
	settings.setValue("rejoin_host", sd.getHostName());
	settings.setValue("rejoin_domain", sd.getDomainName());
}

void CQtClient::closeConnection() {
    m_connection.close();
}

void CQtClient::connectionStatusChanged(enum connectionState status) {
	QString statusMsg;
	if( status == e_connected ) {
		statusMsg = QString("Connected.");
		ui.actionPlayPause->setEnabled( true );
		ui.actionStop->setEnabled( true );
		ui.actionClose->setEnabled(true);
		ui.actionOpen->setEnabled( false );

//		ui.collectionView->setEnabled( true );
//		ui.playlistView->setEnabled( true );
//		ui.nextToPlayView->setEnabled( true );

	}
	else {
		statusMsg = QString("Not Connected.");
		ui.actionPlayPause->setEnabled( false );
		ui.actionStop->setEnabled( false );
		ui.actionClose->setEnabled(false);
		ui.actionOpen->setEnabled( true );

//		ui.collectionView->setEnabled( false );
//		ui.playlistView->setEnabled( false );
//		ui.nextToPlayView->setEnabled( false );

	}
	m_connection_status_label.setText(statusMsg);
}

void CQtClient::progress(int done, int total)
{
	int done_min = done / 60;
	int done_sec = done - done_min * 60;

	int total_min = total / 60;
	int total_sec = total - total_min * 60;

	QString progLabel = QString("%1:%2 / %3:%4").arg(done_min).arg(done_sec, 2, 10, QChar('0')).arg(total_min).arg(total_sec, 2, 10, QChar('0'));
	ui.progressLabel->setText( progLabel );

	ui.posSlider->setValue((done * 100) / total);

}


void CQtClient::setupServiceBrowser()
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
