#include "CQtClient.h"
#include "CServiceBrowser.h"
#include "CPreferencesDlg.h"

#include "CMuroaTreeModel.h"
#include "CMuroaListModel.h"

#include "CDiffBuilder.h"

using namespace muroa;

CQtClient::CQtClient(QWidget *parent)
    : QMainWindow(parent) ,
      m_sessionAdminDlg(&m_session, this),
      m_dnssd("_muroa._tcp")
{
	ui.setupUi(this);

	const CConnection* connPtr = m_session.getConnection();

	ui.playBtn->setDefaultAction(ui.actionPlayPause);
	ui.stopBtn->setDefaultAction(ui.actionStop);
	ui.nextBtn->setDefaultAction(ui.actionNext);
	ui.prevBtn->setDefaultAction(ui.actionPrevious);

	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    connect(ui.actionClose, SIGNAL(triggered()), &m_session, SLOT(closeConnection()));

    connect(ui.actionPlayPause, SIGNAL(triggered()), connPtr, SLOT(play()));
    connect(ui.actionStop, SIGNAL(triggered()), connPtr, SLOT(stop()));
    connect(ui.actionNext, SIGNAL(triggered()), connPtr, SLOT(next()));
    connect(ui.actionPrevious, SIGNAL(triggered()), connPtr, SLOT(prev()));

    connect(ui.actionScanCollection, SIGNAL(triggered()), &m_session, SLOT(scanCollection()));
    connect(ui.actionDump_to_stdout, SIGNAL(triggered()), &m_session, SLOT(dumpCollection()));

    connect(ui.action_Preferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
    connect(ui.actionSession_Admininstration, SIGNAL(triggered()), this, SLOT(showSessionAdmin()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(connPtr, SIGNAL(connectionStatusChanged(enum connectionState)), this, SLOT(connectionStatusChanged(enum connectionState)));

	setupServiceBrowser();

	connect(connPtr, SIGNAL(progressSig(int, int)), this, SLOT(progress(int,int)));
	connectionStatusChanged( e_disconnected );

	m_session.restore();

	ui.mediaColTreeView->setModel(m_session.getMediaColModel());
	ui.playlistListView->setModel(m_session.getPlaylistModel());
	ui.nextlistListView->setModel(m_session.getNextlistModel());

	m_diffBuilder = new CDiffBuilder(m_session.getMediaColModel(), m_session.getPlaylistModel()->getRootItem(), m_session.getNextlistModel()->getRootItem());
	ui.mediaColTreeView->setDiffBuilderPtr(m_diffBuilder);
	ui.playlistListView->setDiffBuilderPtr(m_diffBuilder);
	ui.nextlistListView->setDiffBuilderPtr(m_diffBuilder);

	connect(&m_dnssd, SIGNAL(notifyService( const CServiceDesc&)),
			&m_session, SLOT(openConnection(const CServiceDesc&)));

	connect(m_diffBuilder, SIGNAL(sendCommand(CmdBase*)), m_session.getConnection(), SLOT(sendCommand(CmdBase*)));
}

CQtClient::~CQtClient()
{
	m_session.save();

	delete m_diffBuilder;
	delete m_serviceBrowser;
}

void CQtClient::showPreferences()
{
	CPreferencesDlg prefs;
	prefs.exec();
}

void CQtClient::showSessionAdmin()
{
	m_sessionAdminDlg.show();
}


void CQtClient::openConnection() {
	int rc = m_serviceBrowser->exec();

	int index = -1;
	if(rc == QDialog::Accepted) {
		index = m_serviceBrowser->getSelectedIndex();
	}

	if(index != -1) {
		CServiceDesc* sd = m_dnssd.getService(index);
		m_session.openConnection(*sd);
	}
}

void CQtClient::connectionStatusChanged(enum connectionState status) {
	QString statusMsg;
	if( status == e_connected ) {
		statusMsg = QString("Connected.");
		ui.actionPlayPause->setEnabled( true );
		ui.actionStop->setEnabled( true );
		ui.actionClose->setEnabled(true);
		ui.actionOpen->setEnabled( false );

		ui.mediaColTreeView->setEnabled( true );
		ui.playlistListView->setEnabled( true );
		ui.nextlistListView->setEnabled( true );

	}
	else {
		statusMsg = QString("Not Connected.");
		ui.actionPlayPause->setEnabled( false );
		ui.actionStop->setEnabled( false );
		ui.actionClose->setEnabled(false);
		ui.actionOpen->setEnabled( true );

		ui.mediaColTreeView->setEnabled( false );
		ui.playlistListView->setEnabled( false );
		ui.nextlistListView->setEnabled( false );

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
	connect(&m_dnssd, SIGNAL(notifyService(const CServiceDesc&)), &m_session, SLOT(openConnection(const CServiceDesc&)));
}
